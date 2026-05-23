#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00
#include "../external/httplib.h"
#include "../include/GameController.h"
#include "../include/ReviewDAO.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Auxiliares para JSON
string escapeJsonString(const string& input) {
    string output;
    for (char c : input) {
        switch (c) {
            case '\"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\b': output += "\\b"; break;
            case '\f': output += "\\f"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    char buf[10];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    output += buf;
                } else {
                    output += c;
                }
        }
    }
    return output;
}

string vectorToJson(const vector<string>& vec) {
    string json = "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        json += "\"" + escapeJsonString(vec[i]) + "\"";
        if (i < vec.size() - 1) json += ",";
    }
    json += "]";
    return json;
}

string gameToJson(const Game& g) {
    string json = "{";
    json += "\"appid\":" + to_string(g.appid) + ",";
    json += "\"name\":\"" + escapeJsonString(g.name) + "\",";
    json += "\"release_date\":\"" + to_string(g.release_date.year) + "-" + to_string(g.release_date.month) + "-" + to_string(g.release_date.day) + "\",";
    json += "\"developer\":\"" + escapeJsonString(g.developer) + "\",";
    json += "\"publisher\":\"" + escapeJsonString(g.publisher) + "\",";
    json += "\"platforms\":\"" + escapeJsonString(g.platforms) + "\",";
    json += "\"required_age\":" + to_string(g.required_age) + ",";
    json += "\"achievements\":" + to_string(g.achievements) + ",";
    json += "\"positive_ratings\":" + to_string(g.positive_ratings) + ",";
    json += "\"negative_ratings\":" + to_string(g.negative_ratings) + ",";
    json += "\"average_playtime\":" + to_string(g.average_playtime) + ",";
    json += "\"median_playtime\":" + to_string(g.median_playtime) + ",";
    json += "\"owners\":\"" + escapeJsonString(g.owners) + "\",";
    json += "\"price\":" + to_string(g.price) + ",";
    json += "\"english\":" + string(g.english ? "true" : "false") + ",";
    json += "\"categories\":" + vectorToJson(g.categories) + ",";
    json += "\"genres\":" + vectorToJson(g.genres) + ",";
    json += "\"steamspy_tags\":" + vectorToJson(g.steamspy_tags);
    json += "}";
    return json;
}

string reviewToJson(const Review& r) {
    string json = "{";
    json += "\"idReview\":" + to_string(r.idReview) + ",";
    json += "\"usuario\":\"" + escapeJsonString(r.usuario) + "\",";
    json += "\"comentario\":\"" + escapeJsonString(r.comentario) + "\",";
    json += "\"nota\":" + to_string(r.nota);
    json += "}";
    return json;
}

// Parser JSON simples para nível único, bem básico
string extractJsonField(const string& json, const string& key) {
    string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == string::npos) return "";
    pos += search.length();
    
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
    
    if (json[pos] == '"') { // Valor string
        pos++;
        size_t end = json.find("\"", pos);
        return json.substr(pos, end - pos);
    } else { // Valor numérico/booleano
        size_t end = json.find_first_of(",}", pos);
        return json.substr(pos, end - pos);
    }
}

GameController::GameController(const string& binFilename, const string& userBin, const string& libBin) 
    : binFilename(binFilename), dao(binFilename), uDao(userBin), lDao(libBin) {}

void GameController::run() {
    httplib::Server svr;

    // Serve arquivos estáticos do diretório 'public'
    svr.set_mount_point("/", "./public");

    // Buscar por Nome
    svr.Get("/api/searchByName", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("name")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing name parameter\"}", "application/json");
            return;
        }
        string name = req.get_param_value("name");
        vector<Game> games = dao.searchAllByName(name);

        if (!games.empty()) {
            string json = "{\"games\":[";
            for(size_t i = 0; i < games.size(); i++) {
                json += gameToJson(games[i]);
                if(i < games.size() - 1) json += ",";
            }
            json += "]}";
            res.set_content(json, "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Game not found\"}", "application/json");
        }
    });

    // Buscar por ID
    svr.Get("/api/searchById", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("id")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing id parameter\"}", "application/json");
            return;
        }
        int id = stoi(req.get_param_value("id"));
        Game g;
        long pos;

        if (dao.searchById(id, g, pos)) {
            ReviewDAO reviewDAO("reviews.bin");
            vector<Review> reviews = reviewDAO.buscarPorJogo(g.getAppId());
            
            string json = "{\"game\":" + gameToJson(g) + ", \"reviews\":[";
            for(size_t i = 0; i < reviews.size(); i++) {
                json += reviewToJson(reviews[i]);
                if(i < reviews.size() - 1) json += ",";
            }
            json += "]}";
            
            res.set_content(json, "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Game not found\"}", "application/json");
        }
    });

    // Deletar Jogo (com Integridade Referencial em Cascata)
    svr.Delete("/api/game", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("name")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing name parameter\"}", "application/json");
            return;
        }
        string name = req.get_param_value("name");
        
        // Buscar o jogo para obter o appid antes de deletar
        Game g;
        long pos;
        if (dao.searchByName(name, g, pos)) {
            int appid = g.getAppId();
            
            // Cascata: remove entries da biblioteca e reviews associadas
            int libRemoved = lDao.removerPorJogo(appid);
            ReviewDAO rDao("reviews.bin");
            int revRemoved = rDao.removerPorJogo(appid);
            
            dao.remove(name);
            
            res.set_content("{\"message\":\"Game deleted successfully\", \"libraryEntriesRemoved\":" 
                + to_string(libRemoved) + ", \"reviewsRemoved\":" + to_string(revRemoved) + "}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Game not found\"}", "application/json");
        }
    });

    // Adicionar Jogo
    svr.Post("/api/game", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            string body = req.body;
            Game g;
            
            string idStr = extractJsonField(body, "appid");
            g.appid = idStr.empty() ? dao.getNextAppId() : stoi(idStr);
            
            g.name = extractJsonField(body, "name");
            g.developer = extractJsonField(body, "developer");
            g.publisher = extractJsonField(body, "publisher");
            g.platforms = extractJsonField(body, "platforms");
            
            string priceStr = extractJsonField(body, "price");
            g.price = priceStr.empty() ? 0.0f : stof(priceStr);
            
            string ageStr = extractJsonField(body, "required_age");
            g.required_age = ageStr.empty() ? 0 : stoi(ageStr);
            
            g.owners = extractJsonField(body, "owners");
            
            string avgStr = extractJsonField(body, "average_playtime");
            g.average_playtime = avgStr.empty() ? 0 : stoi(avgStr);

            string medStr = extractJsonField(body, "median_playtime");
            g.median_playtime = medStr.empty() ? 0 : stoi(medStr);

            string engStr = extractJsonField(body, "english");
            g.english = (engStr == "true" || engStr == "1");

            // Tratamento simples para strings separadas por vírgula em campos JSON para vetores
            auto split = [](string s) {
                vector<string> res;
                stringstream ss(s);
                string item;
                while (getline(ss, item, ',')) {
                    if(!item.empty()) res.push_back(item);
                }
                return res;
            };

            g.categories = split(extractJsonField(body, "categories"));
            g.genres = split(extractJsonField(body, "genres"));
            g.steamspy_tags = split(extractJsonField(body, "steamspy_tags"));

            // Apenas campos básicos necessários inicializados aqui
            g.setActive(true);
            
            dao.create(g);
            res.set_content("{\"message\":\"Game added successfully\", \"appid\":" + to_string(g.appid) + "}", "application/json");
        } catch (exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid payload\"}", "application/json");
        }
    });

    // Atualizar Jogo
    svr.Put("/api/game", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            if (!req.has_param("name")) {
                res.status = 400;
                res.set_content("{\"error\":\"Missing name parameter for update\"}", "application/json");
                return;
            }
            string name = req.get_param_value("name");
            Game g;
            long pos;
            if(!dao.searchByName(name, g, pos)){
                res.status = 404;
                res.set_content("{\"error\":\"Game not found\"}", "application/json");
                return;
            }

            string body = req.body;
            
            string newName = extractJsonField(body, "name");
            if (!newName.empty()) g.name = newName;
            
            string dev = extractJsonField(body, "developer");
            if (!dev.empty()) g.developer = dev;

            string pub = extractJsonField(body, "publisher");
            if (!pub.empty()) g.publisher = pub;
            
            string priceStr = extractJsonField(body, "price");
            if (!priceStr.empty()) g.price = stof(priceStr);

            string ageStr = extractJsonField(body, "required_age");
            if (!ageStr.empty()) g.required_age = stoi(ageStr);

            string avgStr = extractJsonField(body, "average_playtime");
            if (!avgStr.empty()) g.average_playtime = stoi(avgStr);

            string medStr = extractJsonField(body, "median_playtime");
            if (!medStr.empty()) g.median_playtime = stoi(medStr);

            string achStr = extractJsonField(body, "achievements");
            if (!achStr.empty()) g.achievements = stoi(achStr);

            string posStr = extractJsonField(body, "positive_ratings");
            if (!posStr.empty()) g.positive_ratings = stoi(posStr);

            string negStr = extractJsonField(body, "negative_ratings");
            if (!negStr.empty()) g.negative_ratings = stoi(negStr);

            string engStr = extractJsonField(body, "english");
            if (!engStr.empty()) g.english = (engStr == "true" || engStr == "1");

            string ownersStr = extractJsonField(body, "owners");
            if (!ownersStr.empty()) g.owners = ownersStr;

            string platStr = extractJsonField(body, "platforms");
            if (!platStr.empty()) g.platforms = platStr;

            auto split = [](string s) {
                vector<string> res;
                stringstream ss(s);
                string item;
                while (getline(ss, item, ',')) {
                    if(!item.empty()) res.push_back(item);
                }
                return res;
            };

            string catStr = extractJsonField(body, "categories");
            if (!catStr.empty()) g.categories = split(catStr);

            string genStr = extractJsonField(body, "genres");
            if (!genStr.empty()) g.genres = split(genStr);

            string tagsStr = extractJsonField(body, "steamspy_tags");
            if (!tagsStr.empty()) g.steamspy_tags = split(tagsStr);
            
            if (dao.update(name, g)) {
                res.set_content("{\"message\":\"Game updated successfully\"}", "application/json");
            } else {
                res.status = 500;
                res.set_content("{\"error\":\"Failed to update game\"}", "application/json");
            }
        } catch (exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid payload\"}", "application/json");
        }
    });

    // Buscar reviews de um jogo específico
    svr.Get("/api/reviews", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("idJogo")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing idJogo\"}", "application/json");
            return;
        }
        int idJogo = stoi(req.get_param_value("idJogo"));
        ReviewDAO rDao("reviews.bin");
        auto reviews = rDao.buscarPorJogo(idJogo);
        
        string json = "[";
        for(size_t i = 0; i < reviews.size(); i++) {
            json += reviewToJson(reviews[i]);
            if(i < reviews.size() - 1) json += ",";
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    // Criar nova review
    svr.Post("/api/review", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            string body = req.body;
            Review r;
            string idJogoStr = extractJsonField(body, "idJogo");
            if (idJogoStr.empty()) throw runtime_error("Missing idJogo");
            
            r.idJogo = stoi(idJogoStr);
            r.usuario = extractJsonField(body, "usuario");
            r.comentario = extractJsonField(body, "comentario");
            
            string notaStr = extractJsonField(body, "nota");
            r.nota = notaStr.empty() ? 0.0f : stof(notaStr);
            r.setAtivo(true);

            ReviewDAO rDao("reviews.bin");
            rDao.criar(r);
            res.set_content("{\"message\":\"Review adicionada!\", \"idReview\":" + to_string(r.idReview) + "}", "application/json");
        } catch (exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Failed to add review\"}", "application/json");
        }
    });

    // --- FASE 3: BUSCA POR INTERVALO (ÁRVORE B+) ---
    svr.Get("/api/games/range", [this](const httplib::Request& req, httplib::Response& res) {
        float min = req.has_param("min") ? stof(req.get_param_value("min")) : 0.0f;
        float max = req.has_param("max") ? stof(req.get_param_value("max")) : 999.0f;
        
        auto games = dao.searchByPriceRange(min, max);
        string json = "{\"games\":[";
        for(size_t i = 0; i < games.size(); i++) {
            json += gameToJson(games[i]);
            if(i < games.size() - 1) json += ",";
        }
        json += "]}";
        res.set_content(json, "application/json");
    });

    // --- FASE 3: USUÁRIOS E BIBLIOTECA (N:N) ---
    svr.Get("/api/users", [this](const httplib::Request& req, httplib::Response& res) {
        auto users = uDao.listarTodos();
        string json = "[";
        for(size_t i = 0; i < users.size(); i++) {
            json += "{\"id\":" + to_string(users[i].idUser) + ",\"nome\":\"" + users[i].nome + "\"}";
            if(i < users.size() - 1) json += ",";
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    svr.Post("/api/user/add", [this](const httplib::Request& req, httplib::Response& res) {
        string body = req.body;
        User u;
        u.nome = extractJsonField(body, "nome");
        u.email = extractJsonField(body, "email");
        u.setAtivo(true);
        uDao.criar(u);
        res.set_content("{\"id\":" + to_string(u.idUser) + "}", "application/json");
    });

    svr.Post("/api/library/add", [this](const httplib::Request& req, httplib::Response& res) {
        string body = req.body;
        LibraryEntry e;
        e.idUser = stoi(extractJsonField(body, "idUser"));
        e.idGame = stoi(extractJsonField(body, "idGame"));
        e.setAtivo(true);
        lDao.adicionar(e);
        res.set_content("{\"message\":\"Success\"}", "application/json");
    });

    svr.Get("/api/library/user", [this](const httplib::Request& req, httplib::Response& res) {
        int idUser = stoi(req.get_param_value("idUser"));
        auto entries = lDao.buscarPorUsuario(idUser);
        string json = "{\"games\":[";
        for(size_t i = 0; i < entries.size(); i++) {
            Game g;
            long pos;
            if(dao.searchById(entries[i].idGame, g, pos)) {
                json += gameToJson(g);
                if(i < entries.size() - 1) json += ",";
            }
        }
        json += "]}";
        res.set_content(json, "application/json");
    });

    // --- REMOVER JOGO DA BIBLIOTECA ---
    svr.Delete("/api/library/remove", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("idUser") || !req.has_param("idGame")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing idUser or idGame\"}", "application/json");
            return;
        }
        int idUser = stoi(req.get_param_value("idUser"));
        int idGame = stoi(req.get_param_value("idGame"));
        
        if (lDao.remover(idUser, idGame)) {
            res.set_content("{\"message\":\"Removido da biblioteca\"}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Entrada não encontrada\"}", "application/json");
        }
    });

    // --- DELETAR USUÁRIO (com Integridade Referencial) ---
    svr.Delete("/api/user/delete", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("id")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing id\"}", "application/json");
            return;
        }
        int id = stoi(req.get_param_value("id"));
        
        // Cascata: remove todas as entradas da biblioteca deste usuário
        int libRemoved = lDao.removerPorUsuario(id);
        
        if (uDao.deletar(id)) {
            res.set_content("{\"message\":\"Usuário deletado\", \"libraryEntriesRemoved\":" + to_string(libRemoved) + "}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Usuário não encontrado\"}", "application/json");
        }
    });

    // Endpoint de Shutdown (Trata tanto GET quanto POST para compatibilidade)
    auto shutdown_handler = [&](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"message\":\"Desligando...\"}", "application/json");
        cout << "\nRequisição de shutdown recebida. Fechando servidor...\n";
        svr.stop();
    };
    svr.Get("/api/shutdown", shutdown_handler);
    svr.Post("/api/shutdown", shutdown_handler);

    int port = 8080;
    cout << "\n============================================\n";
    cout << "Servidor iniciado! Abra seu navegador em:\n";
    cout << "http://localhost:" << port << "\n";
    cout << "Pressione Ctrl+C para parar.\n";
    cout << "============================================\n";

    svr.listen("0.0.0.0", port);
}

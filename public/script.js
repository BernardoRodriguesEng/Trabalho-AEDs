let currentResults = [];
let currentIndex = 0;

function showSection(sectionId) {
    document.querySelectorAll('section').forEach(s => s.classList.remove('active-section'));
    document.querySelectorAll('.nav-links li').forEach(l => l.classList.remove('active'));
    
    document.getElementById(sectionId + '-section').classList.add('active-section');
    event.currentTarget.classList.add('active');
}

let toastTimeout;
function showToast(message, isError = false) {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.style.borderLeft = `4px solid ${isError ? 'var(--danger)' : 'var(--accent)'}`;
    toast.classList.remove('hidden');
    toast.classList.add('show');
    
    clearTimeout(toastTimeout);
    toastTimeout = setTimeout(() => {
        toast.classList.remove('show');
    }, 2000);
}

async function searchGame() {
    const type = document.getElementById('search-type').value;
    const input = document.getElementById('search-input').value;
    if(!input) return showToast("Digite um termo para busca", true);

    let url = "";
    if (type === 'name') {
        url = `/api/searchByName?name=${encodeURIComponent(input)}`;
    } else if (type === 'name_kmp') {
        url = `/api/searchKMP?name=${encodeURIComponent(input)}`;
    } else if (type === 'name_bm') {
        url = `/api/searchBM?name=${encodeURIComponent(input)}`;
    } else {
        url = `/api/searchById?id=${input}`;
    }
    
    try {
        const res = await fetch(url);
        if(!res.ok) throw new Error("Jogo não encontrado");
        
        const data = await res.json();
        if (type.startsWith('name')) {
            currentResults = data.games || [];
        } else {
            currentResults = data.game ? [data.game] : [];
        }
        
        document.getElementById('benchmark-results').style.display = 'none';
        renderResults();
    } catch (e) {
        showToast(e.message, true);
    }
}

async function benchmarkSearch() {
    const input = document.getElementById('search-input').value;
    if(!input) return showToast("Digite um termo para comparar o benchmark", true);

    const type = document.getElementById('search-type').value;
    if (type === 'id') {
        return showToast("O benchmark é apenas para buscas por texto (Nome)", true);
    }

    try {
        const res = await fetch(`/api/benchmarkSearch?name=${encodeURIComponent(input)}`);
        if(!res.ok) throw new Error("Erro ao realizar benchmark");
        
        const data = await res.json();
        
        // Exibe os resultados
        currentResults = data.games || [];
        renderResults();

        // Atualiza e exibe o modal de benchmark
        document.getElementById('bench-seq').textContent = data.time_sequential.toFixed(2) + ' ms';
        document.getElementById('bench-kmp').textContent = data.time_kmp.toFixed(2) + ' ms';
        document.getElementById('bench-bm').textContent = data.time_bm.toFixed(2) + ' ms';
        document.getElementById('benchmark-results').style.display = 'block';

    } catch (e) {
        showToast(e.message, true);
    }
}

function renderResults() {
    if (currentResults.length === 0) {
        showToast("Nenhum resultado encontrado", true);
        document.getElementById('search-result').classList.add('hidden');
        return;
    }

    currentIndex = 0;
    document.getElementById('search-result').classList.remove('hidden');
    displayGame(currentResults[currentIndex]);
    updateCycleCounter();
    showToast(`Encontrado(s) ${currentResults.length} match(es)!`);
}

async function searchByRange() {
    const min = document.getElementById('min-price').value || 0;
    const max = document.getElementById('max-price').value || 999;
    
    try {
        const res = await fetch(`/api/games/range?min=${min}&max=${max}`);
        if(!res.ok) throw new Error("Erro na busca por faixa de preço");
        
        const data = await res.json();
        currentResults = data.games || [];
        
        if (currentResults.length === 0) {
            showToast("Nenhum jogo nesta faixa de preço", true);
            document.getElementById('search-result').classList.add('hidden');
            return;
        }

        currentIndex = 0;
        document.getElementById('search-result').classList.remove('hidden');
        displayGame(currentResults[currentIndex]);
        updateCycleCounter();
        showToast(`Encontrados ${currentResults.length} jogos.`);
    } catch(e) {
        showToast(e.message, true);
    }
}

function cycleResults(direction) {
    currentIndex += direction;
    if (currentIndex >= currentResults.length) currentIndex = 0;
    if (currentIndex < 0) currentIndex = currentResults.length - 1;
    
    displayGame(currentResults[currentIndex]);
    updateCycleCounter();
}

function updateCycleCounter() {
    document.getElementById('result-counter').textContent = `${currentIndex + 1} de ${currentResults.length}`;
}

async function displayGame(game) {
    if(!game) return;
    
    document.getElementById('res-name').textContent = game.name;
    document.getElementById('res-dev').textContent = game.developer;
    document.getElementById('res-pub').textContent = game.publisher;
    document.getElementById('res-price').textContent = game.price > 0 ? `$${game.price.toFixed(2)}` : 'Grátis';
    document.getElementById('res-appid').textContent = game.appid;
    document.getElementById('res-date').textContent = game.release_date;
    document.getElementById('res-pos').textContent = game.positive_ratings;
    document.getElementById('res-neg').textContent = game.negative_ratings;
    document.getElementById('res-avg').textContent = game.average_playtime + ' min';
    document.getElementById('res-med').textContent = game.median_playtime + ' min';
    document.getElementById('res-owners').textContent = game.owners;
    document.getElementById('res-plat').textContent = game.platforms;

    const renderList = (id, items) => {
        const container = document.getElementById(id);
        container.innerHTML = '';
        if(items && items.length > 0) {
            items.forEach(it => {
                const span = document.createElement('span');
                span.className = 'tag';
                span.textContent = it;
                container.appendChild(span);
            });
        } else {
            container.innerHTML = '<span style="color: var(--text-muted)">Nenhum</span>';
        }
    };

    renderList('res-categories', game.categories);
    renderList('res-genres', game.genres);
    renderList('res-tags', game.steamspy_tags);

    // Carregar Reviews via API (Relacionamento 1:N)
    loadReviews(game.appid);
}

async function loadReviews(appid) {
    const container = document.getElementById('reviews-list');
    container.innerHTML = '<div class="loader">Carregando avaliações...</div>';
    
    try {
        const res = await fetch(`/api/reviews?idJogo=${appid}`);
        if(!res.ok) throw new Error("Failed to load reviews");
        
        const reviews = await res.json();
        container.innerHTML = '';
        
        if(reviews.length > 0) {
            reviews.forEach(rev => {
                const card = document.createElement('div');
                card.className = 'review-card';
                card.innerHTML = `
                    <span class="user">${rev.usuario}</span>
                    <span class="tag">${rev.nota}/10</span>
                    <p class="text">${rev.comentario}</p>
                `;
                container.appendChild(card);
            });
        } else {
            container.innerHTML = '<p style="color: var(--text-muted)">Nenhuma avaliação encontrada.</p>';
        }
    } catch (e) {
        container.innerHTML = '<p style="color: var(--danger)">Erro ao carregar avaliações.</p>';
    }
}

async function addGame() {
    const payload = {
        name: document.getElementById('add-name').value,
        developer: document.getElementById('add-dev').value,
        publisher: document.getElementById('add-pub').value,
        price: parseFloat(document.getElementById('add-price').value) || 0,
        required_age: parseInt(document.getElementById('add-age').value) || 0,
        average_playtime: parseInt(document.getElementById('add-avg').value) || 0,
        median_playtime: parseInt(document.getElementById('add-med').value) || 0,
        owners: document.getElementById('add-owners').value,
        platforms: document.getElementById('add-plat').value,
        categories: document.getElementById('add-categories').value,
        genres: document.getElementById('add-genres').value,
        steamspy_tags: document.getElementById('add-tags').value,
        appid: 0 // O servidor irá auto-atribuir se for 0
    };

    try {
        const res = await fetch('/api/game', {
            method: 'POST',
            body: JSON.stringify(payload)
        });
        
        if(!res.ok) throw new Error("Failed to add game");
        
        showToast("Game added.");
        document.getElementById('add-form').reset();
    } catch(e) {
        showToast(e.message, true);
    }
}

async function loadGameForUpdate() {
    const name = document.getElementById('upd-target-name').value;
    if(!name) return showToast("Enter a game name to load", true);

    try {
        const res = await fetch(`/api/searchByName?name=${encodeURIComponent(name)}`);
        if(!res.ok) throw new Error("Jogo não encontrado");
        
        const data = await res.json();
        const game = data.games[0]; // Pega o primeiro match

        if (!game) throw new Error("Jogo não encontrado");

        document.getElementById('upd-name').value = game.name;
        document.getElementById('upd-dev').value = game.developer;
        document.getElementById('upd-pub').value = game.publisher;
        document.getElementById('upd-price').value = game.price;
        document.getElementById('upd-age').value = game.required_age;
        document.getElementById('upd-avg').value = game.average_playtime;
        document.getElementById('upd-med').value = game.median_playtime;
        document.getElementById('upd-pos').value = game.positive_ratings;
        document.getElementById('upd-neg').value = game.negative_ratings;
        document.getElementById('upd-ach').value = game.achievements;
        document.getElementById('upd-owners').value = game.owners;
        document.getElementById('upd-plat').value = game.platforms;
        document.getElementById('upd-categories').value = game.categories.join(', ');
        document.getElementById('upd-genres').value = game.genres.join(', ');
        document.getElementById('upd-tags').value = game.steamspy_tags.join(', ');

        showToast("Game data loaded!");
    } catch(e) {
        showToast(e.message, true);
    }
}

async function updateGame() {
    const targetName = document.getElementById('upd-target-name').value;
    if(!targetName) return showToast("Enter target game name", true);

    const payload = {};
    const fields = [
        { id: 'upd-name', key: 'name' },
        { id: 'upd-dev', key: 'developer' },
        { id: 'upd-pub', key: 'publisher' },
        { id: 'upd-price', key: 'price', type: 'float' },
        { id: 'upd-age', key: 'required_age', type: 'int' },
        { id: 'upd-avg', key: 'average_playtime', type: 'int' },
        { id: 'upd-med', key: 'median_playtime', type: 'int' },
        { id: 'upd-pos', key: 'positive_ratings', type: 'int' },
        { id: 'upd-neg', key: 'negative_ratings', type: 'int' },
        { id: 'upd-ach', key: 'achievements', type: 'int' },
        { id: 'upd-owners', key: 'owners' },
        { id: 'upd-plat', key: 'platforms' },
        { id: 'upd-categories', key: 'categories' },
        { id: 'upd-genres', key: 'genres' },
        { id: 'upd-tags', key: 'steamspy_tags' }
    ];

    fields.forEach(field => {
        const val = document.getElementById(field.id).value;
        if(val !== "") {
            if(field.type === 'int') payload[field.key] = parseInt(val);
            else if(field.type === 'float') payload[field.key] = parseFloat(val);
            else payload[field.key] = val;
        }
    });

    try {
        const res = await fetch(`/api/game?name=${encodeURIComponent(targetName)}`, {
            method: 'PUT',
            body: JSON.stringify(payload)
        });
        
        if(!res.ok) throw new Error("Failed to update game");
        showToast("Game updated.");
    } catch(e) {
        showToast(e.message, true);
    }
}

async function postReview() {
    const appid = document.getElementById('res-appid').textContent;
    if(!appid || appid === "0") return showToast("Selecione um jogo primeiro", true);

    const payload = {
        idJogo: parseInt(appid),
        usuario: document.getElementById('rev-user').value,
        nota: parseInt(document.getElementById('rev-nota').value) || 0,
        comentario: document.getElementById('rev-comment').value
    };

    if(!payload.usuario || !payload.comentario) return showToast("Preencha todos os campos", true);

    try {
        const res = await fetch('/api/review', {
            method: 'POST',
            body: JSON.stringify(payload)
        });
        
        if(!res.ok) throw new Error("Falha ao enviar avaliação");
        
        showToast("Avaliação enviada!");
        document.getElementById('rev-user').value = '';
        document.getElementById('rev-nota').value = '';
        document.getElementById('rev-comment').value = '';
        
        // Recarrega as reviews
        loadReviews(appid);
    } catch(e) {
        showToast(e.message, true);
    }
}

async function deleteGame() {
    const name = document.getElementById('del-name').value;
    if(!name) return showToast("Enter game name", true);

    if(!confirm(`Are you sure you want to delete ${name}?`)) return;

    try {
        const res = await fetch(`/api/game?name=${encodeURIComponent(name)}`, {
            method: 'DELETE'
        });
        
        if(!res.ok) throw new Error("Failed to delete game");
        showToast("Game deleted.");
        document.getElementById('del-name').value = '';
    } catch(e) {
        showToast(e.message, true);
    }
}

// --- FASE 3: USUÁRIOS E BIBLIOTECA ---

async function createUser() {
    const nome = document.getElementById('new-user-name').value;
    const email = document.getElementById('new-user-email').value;
    if(!nome || !email) return showToast("Preencha todos os campos", true);

    try {
        const res = await fetch('/api/user/add', {
            method: 'POST',
            body: JSON.stringify({ nome, email })
        });
        if(!res.ok) throw new Error("Erro ao criar usuário");
        
        showToast("Usuário criado!");
        document.getElementById('new-user-name').value = '';
        document.getElementById('new-user-email').value = '';
        loadUsers();
    } catch(e) {
        showToast(e.message, true);
    }
}

async function loadUsers() {
    try {
        const res = await fetch('/api/users');
        const users = await res.json();
        const selector = document.getElementById('user-selector');
        selector.innerHTML = '<option value="0">Visitante</option>';
        
        users.forEach(u => {
            const opt = document.createElement('option');
            opt.value = u.id;
            opt.textContent = u.nome;
            selector.appendChild(opt);
        });
    } catch(e) { console.error("Erro ao carregar usuários"); }
}

function switchUser() {
    const selector = document.getElementById('user-selector');
    const id = selector.value;
    const name = selector.options[selector.selectedIndex].textContent;
    
    document.getElementById('active-user-id').value = id;
    document.getElementById('active-user-name').textContent = name;
    
    if(id !== "0") {
        loadUserLibrary(id);
        // Preenche o campo de nome na review automaticamente
        document.getElementById('rev-user').value = name;
    } else {
        document.getElementById('user-library-list').innerHTML = '<p class="form-hint">Selecione um usuário para ver seus jogos.</p>';
    }
}

async function addToLibrary() {
    const userId = document.getElementById('active-user-id').value;
    const gameId = document.getElementById('res-appid').textContent;
    
    if(userId === "0") return showToast("Selecione um usuário primeiro!", true);
    if(!gameId || gameId === "0") return showToast("Busque um jogo primeiro!", true);

    try {
        const res = await fetch('/api/library/add', {
            method: 'POST',
            body: JSON.stringify({ idUser: parseInt(userId), idGame: parseInt(gameId) })
        });
        if(!res.ok) throw new Error("Erro ao adicionar à biblioteca");
        
        showToast("Jogo adicionado à sua biblioteca!");
        loadUserLibrary(userId);
    } catch(e) {
        showToast(e.message, true);
    }
}

async function removeFromLibrary(idUser, idGame) {
    if(!confirm('Remover este jogo da sua biblioteca?')) return;
    try {
        const res = await fetch(`/api/library/remove?idUser=${idUser}&idGame=${idGame}`, {
            method: 'DELETE'
        });
        if(!res.ok) throw new Error("Erro ao remover da biblioteca");
        showToast("Jogo removido da biblioteca!");
        loadUserLibrary(idUser);
    } catch(e) {
        showToast(e.message, true);
    }
}

async function deleteUser() {
    const selector = document.getElementById('user-selector');
    const userId = selector.value;
    if(userId === "0") return showToast("Selecione um usuário para deletar", true);
    const userName = selector.options[selector.selectedIndex].textContent;
    if(!confirm(`Deletar o usuário "${userName}" e toda a sua biblioteca?`)) return;

    try {
        const res = await fetch(`/api/user/delete?id=${userId}`, { method: 'DELETE' });
        if(!res.ok) throw new Error("Erro ao deletar usuário");
        showToast("Usuário deletado!");
        document.getElementById('active-user-id').value = '0';
        document.getElementById('active-user-name').textContent = 'Visitante';
        loadUsers();
        document.getElementById('user-library-list').innerHTML = '<p class="form-hint">Selecione um usuário para ver seus jogos.</p>';
    } catch(e) {
        showToast(e.message, true);
    }
}

async function loadUserLibrary(userId) {
    const container = document.getElementById('user-library-list');
    container.innerHTML = '<div class="loader">Carregando biblioteca...</div>';
    
    try {
        const res = await fetch(`/api/library/user?idUser=${userId}`);
        const data = await res.json();
        container.innerHTML = '';
        
        if(data.games && data.games.length > 0) {
            data.games.forEach(game => {
                const card = document.createElement('div');
                card.className = 'library-card glass-panel';
                card.innerHTML = `
                    <div class="library-card-info">
                        <strong>${game.name}</strong>
                        <small>${game.developer}</small>
                        <span class="tag price">${game.price > 0 ? '$' + game.price.toFixed(2) : 'Grátis'}</span>
                    </div>
                    <button class="btn-small btn-remove" onclick="removeFromLibrary(${userId}, ${game.appid})">✕ Remover</button>
                `;
                container.appendChild(card);
            });
        } else {
            container.innerHTML = '<p class="form-hint">Sua biblioteca está vazia.</p>';
        }
    } catch(e) { container.innerHTML = '<p class="text-danger">Erro ao carregar biblioteca.</p>'; }
}

// Inicialização
document.addEventListener('DOMContentLoaded', () => {
    loadUsers();
});



// --- COMPRESSÃO ---

async function compressDB(type) {
    showToast(`Iniciando compressão ${type.toUpperCase()}... aguarde.`);
    try {
        const res = await fetch('/api/compress', {
            method: 'POST',
            body: JSON.stringify({ type })
        });
        if(!res.ok) throw new Error(`Erro na compressão ${type}`);
        
        const data = await res.json();
        
        document.getElementById(`${type}-orig-size`).textContent = data.originalSize;
        document.getElementById(`${type}-comp-size`).textContent = data.compressedSize;
        document.getElementById(`${type}-taxa`).textContent = data.taxa.toFixed(2);
        
        document.getElementById(`${type}-stats`).classList.remove('hidden');
        showToast(`Compressão ${type.toUpperCase()} concluída.`);
    } catch(e) {
        showToast(e.message, true);
    }
}

async function decompressDB(type) {
    showToast(`Iniciando descompressão ${type.toUpperCase()}... aguarde.`);
    try {
        const res = await fetch('/api/decompress', {
            method: 'POST',
            body: JSON.stringify({ type })
        });
        if(!res.ok) throw new Error(`Erro na descompressão ${type}`);
        
        const data = await res.json();
        
        document.getElementById(`${type}-rest-size`).textContent = data.restoredSize;
        
        document.getElementById(`${type}-rest-stats`).classList.remove('hidden');
        showToast(`Descompressão ${type.toUpperCase()} concluída.`);
    } catch(e) {
        showToast(e.message, true);
    }
}

// --- CRIPTOGRAFIA ---
async function encryptDB() {
    showToast(`Iniciando Criptografia OTP... aguarde.`);
    try {
        const res = await fetch('/api/encrypt', { method: 'POST' });
        const data = await res.json();
        if(!res.ok) throw new Error(data.error || "Erro na criptografia");
        showToast(data.message);
    } catch(e) {
        showToast(e.message, true);
    }
}

async function decryptDB() {
    showToast(`Iniciando Descriptografia OTP... aguarde.`);
    try {
        const res = await fetch('/api/decrypt', { method: 'POST' });
        const data = await res.json();
        if(!res.ok) throw new Error(data.error || "Erro na descriptografia");
        showToast(data.message);
    } catch(e) {
        showToast(e.message, true);
    }
}

window.addEventListener('beforeunload', function () { 
    navigator.sendBeacon('/api/shutdown'); 
});

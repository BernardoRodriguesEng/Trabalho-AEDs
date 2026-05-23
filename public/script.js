let currentResults = [];
let currentIndex = 0;

function showSection(sectionId) {
    document.querySelectorAll('section').forEach(s => s.classList.remove('active-section'));
    document.querySelectorAll('.nav-links li').forEach(l => l.classList.remove('active'));
    
    document.getElementById(sectionId + '-section').classList.add('active-section');
    event.currentTarget.classList.add('active');
}

function showToast(message, isError = false) {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.style.borderLeft = `4px solid ${isError ? 'var(--danger)' : 'var(--accent)'}`;
    toast.classList.remove('hidden');
    toast.classList.add('show');
    
    setTimeout(() => {
        toast.classList.remove('show');
    }, 3000);
}

async function searchGame() {
    const type = document.getElementById('search-type').value;
    const query = document.getElementById('search-input').value;
    
    if(!query) return showToast("Please enter a search query", true);

    try {
        const url = type === 'name' 
            ? `/api/searchByName?name=${encodeURIComponent(query)}`
            : `/api/searchById?id=${encodeURIComponent(query)}`;
            
        const res = await fetch(url);
        if(!res.ok) throw new Error("Game not found");
        
        const data = await res.json();
        
        if (type === 'name') {
            currentResults = data.games;
        } else {
            currentResults = [data.game];
        }
        
        currentIndex = 0;
        displayGame(currentResults[currentIndex]);
        
        const cycleControls = document.querySelector('.cycle-controls');
        if (currentResults.length > 1) {
            cycleControls.classList.remove('hidden');
            updateCycleCounter();
        } else {
            cycleControls.classList.add('hidden');
        }

        document.getElementById('search-result').classList.remove('hidden');
        showToast(`Encontrado(s) ${currentResults.length} match(es)!`);
    } catch(e) {
        document.getElementById('search-result').classList.add('hidden');
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
        
        showToast("Game added successfully!");
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
        showToast("Game updated successfully!");
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
        
        showToast("Avaliação enviada com sucesso!");
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
        showToast("Game deleted successfully!");
        document.getElementById('del-name').value = '';
    } catch(e) {
        showToast(e.message, true);
    }
}



window.addEventListener('beforeunload', function () { 
    navigator.sendBeacon('/api/shutdown'); 
});

// Клиентская логика сайта: работа с API приложения по относительному пути /api/.
// Запросы уходят на тот же хост/порт (Apache), а Apache проксирует их в контейнер app.

const API = "/api";

const form = document.getElementById("note-form");
const statusEl = document.getElementById("status");
const tbody = document.querySelector("#notes tbody");

function setStatus(text, kind) {
    statusEl.textContent = text;
    statusEl.className = "status" + (kind ? " " + kind : "");
}

function escapeHtml(value) {
    return String(value)
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;");
}

function render(notes) {
    if (!notes.length) {
        tbody.innerHTML = '<tr><td colspan="4">Записей пока нет — добавьте первую.</td></tr>';
        return;
    }
    tbody.innerHTML = notes.map((n) => `
        <tr>
            <td>${n.id}</td>
            <td>${escapeHtml(n.title)}</td>
            <td>${escapeHtml(n.body)}</td>
            <td>${escapeHtml(n.created_at)}</td>
        </tr>
    `).join("");
}

async function loadNotes() {
    try {
        const response = await fetch(`${API}/notes`);
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        const data = await response.json();
        render(data.notes || []);
        setStatus(`Загружено записей: ${(data.notes || []).length}`, "ok");
    } catch (err) {
        setStatus(`Не удалось получить список записей: ${err.message}`, "err");
    }
}

form.addEventListener("submit", async (event) => {
    event.preventDefault();
    const title = document.getElementById("title").value.trim();
    const body = document.getElementById("body").value.trim();
    if (!title || !body) {
        setStatus("Заполните заголовок и текст", "err");
        return;
    }

    try {
        const response = await fetch(`${API}/notes`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ title, body }),
        });
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || `HTTP ${response.status}`);
        form.reset();
        setStatus(`Запись №${data.note.id} сохранена в MySQL`, "ok");
        await loadNotes();
    } catch (err) {
        setStatus(`Ошибка сохранения: ${err.message}`, "err");
    }
});

loadNotes();

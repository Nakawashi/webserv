// JavaScript pour la redirection automatique
let countdown = 5; // Nombre de secondes avant la redirection
const countdownDisplay = document.getElementById('countdown');

function updateCountdown() {
    countdown--;
    countdownDisplay.textContent = countdown;

    if (countdown <= 0) {
        window.location.href = '/public/index.html'; // Rediriger vers la page d'accueil
    }
}

// Mettre à jour le compte à rebours toutes les 1 seconde
setInterval(updateCountdown, 1000);

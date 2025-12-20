#ifndef _INC_BOT_AI_HPP
#define _INC_BOT_AI_HPP

#include "../../Common.hpp"
#include <functional>

// Niveaux de difficulté des bots
enum class BotDifficulty
{
    EASY,     // Décisions aléatoires, réactions lentes
    MEDIUM,   // Stratégie basique, réactions moyennes
    HARD      // Stratégie avancée, réactions rapides
};

// Classe de base pour l'IA des bots
class BotAI
{
public:
    BotAI(int playerIndex, BotDifficulty difficulty);
    
    // ==================== BOARD GAME ====================
    // Décision de lancer le dé
    bool ShouldRollDice(float deltaTime);
    
    // Choix de chemin (retourne 0 ou 1)
    int ChoosePath(const std::vector<int>& availablePaths, int currentPosition);
    
    // ==================== ROCK PAPER SCISSORS ====================
    // Choisir pierre/papier/ciseaux (0=rock, 1=paper, 2=scissors)
    int ChooseRPS();
    
    // ==================== ARM WRESTLING ====================
    // Intensité du mashing (0.0 à 1.0)
    float GetArmWrestlingMashIntensity(float currentPosition, float opponentPosition);
    
    // ==================== FLAG GAME ====================
    // Décision de mémorisation (timing)
    bool ShouldMemorizeFlag(float displayTime, int flagComplexity);
    
    // Choix de drapeau mémorisé (retourne l'index)
    int ChooseFlagAnswer(const std::vector<int>& options, int correctAnswer, bool hasSeenFlag);
    
    // ==================== RAND CARD ====================
    // Décision de tirer une carte
    bool ShouldDrawCard(float deltaTime, int currentCardValue);
    
    // Évaluation du risque (0.0 = pas de risque, 1.0 = très risqué)
    float EvaluateCardRisk(int currentTotal, int cardsRemaining);
    
    // ==================== ROULETTE RUSSE ====================
    // Décision de tirer (basée sur probabilité et pression)
    bool ShouldPullTrigger(float deltaTime, int shotsRemaining, int totalChambers, float pressure);
    
    // Évaluation de la peur/hésitation (affecte le timing)
    float GetRussianRouletteFear(int shotsRemaining, int totalChambers);
    
    // ==================== BASKET ====================
    // Calcul de la force de lancer (0.0 à 1.0)
    float CalculateBasketThrowForce(float distanceToTarget, float currentForce);
    
    // Décision de lancer
    bool ShouldThrowBasket(float currentForce, float optimalForce);
    
    // ==================== HELPERS GÉNÉRAUX ====================
    // Temps de réflexion avant action (pour rendre naturel)
    float GetThinkingTime() const;
    
    // Réaction à un événement visuel (temps de réaction)
    float GetReactionTime() const;
    
    // Chance de faire une erreur
    bool MakeMistake() const;
    
    BotDifficulty GetDifficulty() const { return m_difficulty; }
    int GetPlayerIndex() const { return m_playerIndex; }
    
    // Réinitialiser les timers pour un nouveau tour
    void ResetForNewTurn();

private:
    int m_playerIndex;
    BotDifficulty m_difficulty;
    float m_thinkingTimer;
    float m_thinkingDuration;
    float m_reactionTime;
    bool m_decisionMade;
    
    // Mémoire temporaire pour les mini-jeux
    struct BotMemory
    {
        int lastRPSChoice;        // Dernier choix RPS
        int consecutiveWins;      // Victoires consécutives
        int consecutiveLosses;    // Défaites consécutives
        bool hasSeenFlag;         // A vu le drapeau
        int memorizedFlag;        // Index du drapeau mémorisé
        float cardRiskTolerance;  // Tolérance au risque (cartes)
        float roulettePressure;   // Niveau de stress (roulette)
    };
    
    BotMemory m_memory;
    
    // Helpers privés
    void ResetThinkingTimer();
    void UpdateDifficulty();
    
    // Stratégies spécifiques
    int MakeSmartPathChoice(const std::vector<int>& paths, int currentPos);
    int MakeRandomPathChoice(const std::vector<int>& paths);
    int CounterRPS(int opponentLastChoice);  // Contre-stratégie RPS
    float CalculateOptimalBasketForce(float distance);
};

#endif // _INC_BOT_AI_HPP
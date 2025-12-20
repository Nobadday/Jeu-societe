#include "BotAI.hpp"

BotAI::BotAI(int playerIndex, BotDifficulty difficulty)
    : m_playerIndex(playerIndex)
    , m_difficulty(difficulty)
    , m_thinkingTimer(0.0f)
    , m_thinkingDuration(0.0f)
    , m_reactionTime(0.0f)
    , m_decisionMade(false)
{
    // Initialiser la mémoire
    m_memory.lastRPSChoice = -1;
    m_memory.consecutiveWins = 0;
    m_memory.consecutiveLosses = 0;
    m_memory.hasSeenFlag = false;
    m_memory.memorizedFlag = -1;
    m_memory.cardRiskTolerance = 0.5f;
    m_memory.roulettePressure = 0.0f;
    
    UpdateDifficulty();
    ResetThinkingTimer();
}

void BotAI::UpdateDifficulty()
{
    // Configurer les paramètres selon la difficulté
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        m_reactionTime = randmt::RandomFloat(0.8f, 1.5f);
        m_memory.cardRiskTolerance = randmt::RandomFloat(0.3f, 0.5f);
        break;
        
    case BotDifficulty::MEDIUM:
        m_reactionTime = randmt::RandomFloat(0.4f, 0.9f);
        m_memory.cardRiskTolerance = randmt::RandomFloat(0.5f, 0.7f);
        break;
        
    case BotDifficulty::HARD:
        m_reactionTime = randmt::RandomFloat(0.15f, 0.4f);
        m_memory.cardRiskTolerance = randmt::RandomFloat(0.7f, 0.85f);
        break;
    }
}

void BotAI::ResetThinkingTimer()
{
    // Temps de réflexion selon difficulté
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        m_thinkingDuration = randmt::RandomFloat(1.0f, 2.5f);
        break;
    case BotDifficulty::MEDIUM:
        m_thinkingDuration = randmt::RandomFloat(0.8f, 1.8f);
        break;
    case BotDifficulty::HARD:
        m_thinkingDuration = randmt::RandomFloat(0.5f, 1.2f);
        break;
    }
    
    m_thinkingTimer = 0.0f;
    m_decisionMade = false;
}

void BotAI::ResetForNewTurn()
{
    ResetThinkingTimer();
    m_memory.roulettePressure = 0.0f;
}

// ==================== BOARD GAME ====================

bool BotAI::ShouldRollDice(float deltaTime)
{
    m_thinkingTimer += deltaTime;
    
    if (!m_decisionMade && m_thinkingTimer >= m_thinkingDuration)
    {
        m_decisionMade = true;
        ResetThinkingTimer();
        return true;
    }
    
    return false;
}

int BotAI::ChoosePath(const std::vector<int>& availablePaths, int currentPosition)
{
    if (availablePaths.empty())
        return 0;
    
    if (availablePaths.size() == 1)
        return 0;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        return MakeRandomPathChoice(availablePaths);
        
    case BotDifficulty::MEDIUM:
        // 70% stratégique, 30% aléatoire
        if (randmt::Chance(0.7f))
            return MakeSmartPathChoice(availablePaths, currentPosition);
        else
            return MakeRandomPathChoice(availablePaths);
        
    case BotDifficulty::HARD:
        return MakeSmartPathChoice(availablePaths, currentPosition);
    }
    
    return 0;
}

int BotAI::MakeRandomPathChoice(const std::vector<int>& paths)
{
    return randmt::RandomInt(0, (int)paths.size() - 1);
}

int BotAI::MakeSmartPathChoice(const std::vector<int>& paths, int currentPos)
{
    // Stratégie: préférer le chemin du haut 60% du temps
    // (peut être amélioré avec analyse du plateau)
    if (randmt::Chance(0.6f))
        return 0;
    else
        return (paths.size() > 1) ? 1 : 0;
}

// ==================== ROCK PAPER SCISSORS ====================

int BotAI::ChooseRPS()
{
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        // Choix complètement aléatoire
        return randmt::RandomInt(0, 2);
        
    case BotDifficulty::MEDIUM:
        // 50% aléatoire, 50% contre-stratégie
        if (randmt::Chance(0.5f) && m_memory.lastRPSChoice != -1)
            return CounterRPS(m_memory.lastRPSChoice);
        else
            return randmt::RandomInt(0, 2);
        
    case BotDifficulty::HARD:
        // Contre-stratégie avancée
        if (m_memory.lastRPSChoice != -1)
        {
            // Si on perd plusieurs fois, changer de stratégie
            if (m_memory.consecutiveLosses >= 2)
                return randmt::RandomInt(0, 2);
            else
                return CounterRPS(m_memory.lastRPSChoice);
        }
        return randmt::RandomInt(0, 2);
    }
    
    return 0;
}

int BotAI::CounterRPS(int opponentLastChoice)
{
    // Pierre bat Ciseaux, Papier bat Pierre, Ciseaux bat Papier
    // Prédire que l'adversaire va répéter et contrer
    return (opponentLastChoice + 1) % 3;
}

// ==================== ARM WRESTLING ====================

float BotAI::GetArmWrestlingMashIntensity(float currentPosition, float opponentPosition)
{
    float intensity = 0.0f;
    
    // Calculer l'urgence basée sur la position
    float urgency = (opponentPosition - currentPosition) / 100.0f;
    urgency = std::clamp(urgency, 0.0f, 1.0f);
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        intensity = randmt::RandomFloat(0.3f, 0.6f);
        break;
        
    case BotDifficulty::MEDIUM:
        // Intensité proportionnelle à l'urgence
        intensity = randmt::RandomFloat(0.5f, 0.8f) + (urgency * 0.2f);
        break;
        
    case BotDifficulty::HARD:
        // Très réactif et adaptatif
        intensity = randmt::RandomFloat(0.7f, 0.95f) + (urgency * 0.3f);
        break;
    }
    
    return std::clamp(intensity, 0.0f, 1.0f);
}

// ==================== FLAG GAME ====================

bool BotAI::ShouldMemorizeFlag(float displayTime, int flagComplexity)
{
    float memorizationChance = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        // 30-50% de chance selon complexité
        memorizationChance = 0.5f - (flagComplexity * 0.05f);
        break;
        
    case BotDifficulty::MEDIUM:
        // 60-80% de chance
        memorizationChance = 0.8f - (flagComplexity * 0.04f);
        break;
        
    case BotDifficulty::HARD:
        // 90-95% de chance
        memorizationChance = 0.95f - (flagComplexity * 0.01f);
        break;
    }
    
    memorizationChance = std::clamp(memorizationChance, 0.0f, 1.0f);
    
    if (randmt::Chance(memorizationChance))
    {
        m_memory.hasSeenFlag = true;
        return true;
    }
    
    return false;
}

int BotAI::ChooseFlagAnswer(const std::vector<int>& options, int correctAnswer, bool hasSeenFlag)
{
    if (options.empty())
        return 0;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        // 40% bonne réponse si mémorisé
        if (hasSeenFlag && randmt::Chance(0.4f))
            return correctAnswer;
        return options[randmt::RandomInt(0, (int)options.size() - 1)];
        
    case BotDifficulty::MEDIUM:
        // 70% bonne réponse si mémorisé
        if (hasSeenFlag && randmt::Chance(0.7f))
            return correctAnswer;
        return options[randmt::RandomInt(0, (int)options.size() - 1)];
        
    case BotDifficulty::HARD:
        // 95% bonne réponse si mémorisé
        if (hasSeenFlag && randmt::Chance(0.95f))
            return correctAnswer;
        return options[randmt::RandomInt(0, (int)options.size() - 1)];
    }
    
    return 0;
}

// ==================== RAND CARD ====================

bool BotAI::ShouldDrawCard(float deltaTime, int currentCardValue)
{
    m_thinkingTimer += deltaTime;
    
    if (m_thinkingTimer < m_thinkingDuration)
        return false;
    
    ResetThinkingTimer();
    
    // Stratégie basée sur la valeur actuelle et la tolérance au risque
    float drawChance = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        // Très aléatoire, peu stratégique
        if (currentCardValue < 15)
            drawChance = 0.8f;
        else if (currentCardValue < 18)
            drawChance = 0.5f;
        else
            drawChance = 0.3f;
        break;
        
    case BotDifficulty::MEDIUM:
        // Stratégie basique
        if (currentCardValue < 14)
            drawChance = 0.9f;
        else if (currentCardValue < 17)
            drawChance = 0.6f;
        else if (currentCardValue < 19)
            drawChance = 0.3f;
        else
            drawChance = 0.1f;
        break;
        
    case BotDifficulty::HARD:
        // Stratégie optimale (comme un joueur de Blackjack)
        if (currentCardValue < 12)
            drawChance = 1.0f;
        else if (currentCardValue < 16)
            drawChance = 0.7f;
        else if (currentCardValue < 18)
            drawChance = 0.4f;
        else if (currentCardValue < 20)
            drawChance = 0.15f;
        else
            drawChance = 0.0f;
        break;
    }
    
    return randmt::Chance(drawChance);
}

float BotAI::EvaluateCardRisk(int currentTotal, int cardsRemaining)
{
    if (currentTotal >= 21)
        return 1.0f;  // Risque maximal
    
    float risk = (float)currentTotal / 21.0f;
    
    // Ajuster selon le nombre de cartes restantes
    if (cardsRemaining < 5)
        risk *= 1.2f;
    
    return std::clamp(risk, 0.0f, 1.0f);
}

// ==================== ROULETTE RUSSE ====================

bool BotAI::ShouldPullTrigger(float deltaTime, int shotsRemaining, int totalChambers, float pressure)
{
    m_thinkingTimer += deltaTime;
    
    // Augmenter la pression avec le temps
    m_memory.roulettePressure += deltaTime * 0.1f;
    
    float minThinkingTime = GetRussianRouletteFear(shotsRemaining, totalChambers);
    
    if (m_thinkingTimer < minThinkingTime)
        return false;
    
    ResetThinkingTimer();
    
    // Calculer la probabilité de tirer
    float survivalOdds = (float)(totalChambers - shotsRemaining) / (float)totalChambers;
    float pullChance = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        // Très hésitant, temps de réflexion long
        pullChance = survivalOdds * 0.4f + m_memory.roulettePressure * 0.3f;
        break;
        
    case BotDifficulty::MEDIUM:
        // Équilibré
        pullChance = survivalOdds * 0.6f + m_memory.roulettePressure * 0.2f;
        break;
        
    case BotDifficulty::HARD:
        // Courageux mais stratégique
        pullChance = survivalOdds * 0.8f + m_memory.roulettePressure * 0.15f;
        break;
    }
    
    pullChance = std::clamp(pullChance, 0.0f, 1.0f);
    
    return randmt::Chance(pullChance);
}

float BotAI::GetRussianRouletteFear(int shotsRemaining, int totalChambers)
{
    float dangerLevel = (float)shotsRemaining / (float)totalChambers;
    
    float fearTime = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        fearTime = 2.0f + dangerLevel * 4.0f;  // 2-6 secondes
        break;
        
    case BotDifficulty::MEDIUM:
        fearTime = 1.5f + dangerLevel * 3.0f;  // 1.5-4.5 secondes
        break;
        
    case BotDifficulty::HARD:
        fearTime = 1.0f + dangerLevel * 2.0f;  // 1-3 secondes
        break;
    }
    
    return fearTime;
}

// ==================== BASKET ====================

float BotAI::CalculateBasketThrowForce(float distanceToTarget, float currentForce)
{
    float optimalForce = CalculateOptimalBasketForce(distanceToTarget);
    
    float error = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        error = randmt::RandomFloat(-0.15f, 0.15f);
        break;
        
    case BotDifficulty::MEDIUM:
        error = randmt::RandomFloat(-0.08f, 0.08f);
        break;
        
    case BotDifficulty::HARD:
        error = randmt::RandomFloat(-0.03f, 0.03f);
        break;
    }
    
    return std::clamp(optimalForce + error, 0.0f, 1.0f);
}

float BotAI::CalculateOptimalBasketForce(float distance)
{
    // Formule approximative: force augmente avec la distance
    return std::clamp(distance / 500.0f, 0.3f, 0.95f);
}

bool BotAI::ShouldThrowBasket(float currentForce, float optimalForce)
{
    float tolerance = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        tolerance = 0.2f;
        break;
        
    case BotDifficulty::MEDIUM:
        tolerance = 0.1f;
        break;
        
    case BotDifficulty::HARD:
        tolerance = 0.05f;
        break;
    }
    
    return std::abs(currentForce - optimalForce) <= tolerance;
}

// ==================== HELPERS GÉNÉRAUX ====================

float BotAI::GetThinkingTime() const
{
    return m_thinkingDuration;
}

float BotAI::GetReactionTime() const
{
    return m_reactionTime;
}

bool BotAI::MakeMistake() const
{
    float mistakeChance = 0.0f;
    
    switch (m_difficulty)
    {
    case BotDifficulty::EASY:
        mistakeChance = 0.25f;  // 25% d'erreur
        break;
        
    case BotDifficulty::MEDIUM:
        mistakeChance = 0.10f;  // 10% d'erreur
        break;
        
    case BotDifficulty::HARD:
        mistakeChance = 0.02f;  // 2% d'erreur
        break;
    }
    
    return randmt::Chance(mistakeChance);
}
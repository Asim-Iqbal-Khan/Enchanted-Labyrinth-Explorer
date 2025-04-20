#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Queue.h"
#include <queue>
#include <unordered_map>
#include "Stack.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Treasure.h"
#include "Inventory.h"

using namespace std;
using namespace sf;

const int DESIRED_COLS = 61;
const int CELL_SIZE = 20;
const int GRID_ROWS = 21;
const int GRID_COLS = DESIRED_COLS * GRID_ROWS / GRID_ROWS;
const int HEIGHT = GRID_ROWS * CELL_SIZE;
const int WIDTH = GRID_COLS * CELL_SIZE;
const int EXIT_ROW = GRID_ROWS - 2;  // Exit row position
const int EXIT_COL = GRID_COLS - 1;  // Exit column position

struct Node_d {
    int row;
    int col;
    int distance;

    bool operator<(const Node_d& other) const {
        return distance > other.distance;
    }
};

class Game {
public:
    char** maze;
    bool isPaused;
    Clock fireClock;
    sf::Texture pauseTexture;
    sf::Sprite pauseSprite;
    sf::Font font;
    sf::Text scoreText;
    int score = 0;
    Game() {
        maze = nullptr;
        if (!loadPauseScreen()) {
            std::exit(1);
        }
        font.loadFromFile("arial.ttf");
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(WIDTH / 2 - 50, HEIGHT + 10);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: 0");
        isPaused = false;
    }

    bool loadPauseScreen() {
        if (!pauseTexture.loadFromFile("img/pause_screen.png")) {
            std::cerr << "Error loading pause screen image" << std::endl;
            return false;
        }
        pauseSprite.setTexture(pauseTexture);
        float scaleFactorX = static_cast<float>(WIDTH) / pauseTexture.getSize().x;
        float scaleFactorY = static_cast<float>(HEIGHT) / pauseTexture.getSize().y;
        pauseSprite.setScale(scaleFactorX, scaleFactorY);

        return true;
    }

    void handleEvents(RenderWindow& window) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
                isPaused = !isPaused;
            }
        }
    }

    void GenerateMaze() {
        srand(time(0));
        int directions[4][2] = {
            {-2, 0},
            {2, 0},
            {0, -2},
            {0, 2}
        };
        Stack<int> cellStack;
        maze = new char* [GRID_ROWS];
        for (int i = 0; i < GRID_ROWS; i++) {
            maze[i] = new char[GRID_COLS];
        }
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                maze[i][j] = '#';
            }
        }
        int startRow = 1;
        int startCol = 1;
        cellStack.push(startRow, startCol);
        maze[startRow][startCol] = ' ';
        while (!cellStack.isEmpty()) {
            int* currentCell = cellStack.peek();
            cellStack.pop();
            Queue<int> validNeighbors;
            for (int i = 0; i < 4; i++) {
                int newRow = currentCell[0] + directions[i][0];
                int newCol = currentCell[1] + directions[i][1];
                if (newRow > 0 && newRow < GRID_ROWS - 1 && newCol > 0 && newCol < GRID_COLS - 1) {
                    if (maze[newRow][newCol] == '#') {
                        validNeighbors.enqueue(newRow, newCol);
                    }
                }
            }

            if (!validNeighbors.is_empty()) {
                cellStack.push(currentCell[0], currentCell[1]);
                int* chosenNeighbor = validNeighbors.getData(rand() % validNeighbors.length);
                int midRow = (currentCell[0] + chosenNeighbor[0]) / 2;
                int midCol = (currentCell[1] + chosenNeighbor[1]) / 2;
                maze[midRow][midCol] = ' ';
                maze[chosenNeighbor[0]][chosenNeighbor[1]] = ' ';
                cellStack.push(chosenNeighbor[0], chosenNeighbor[1]);
            }
        }
        maze[1][0] = 'E';
        maze[GRID_ROWS - 2][GRID_COLS - 1] = 'X';
    }

    Queue<int> FindShortestPath(int* start, int* end) {
        priority_queue<Node_d> pq;
        unordered_map<int, pair<int, int>> parent;
        bool** visited = new bool* [GRID_ROWS];
        for (int i = 0; i < GRID_ROWS; i++) {
            visited[i] = new bool[GRID_COLS] {false};
        }
        pq.push({ start[0], start[1], 0 });
        parent[start[0] * GRID_COLS + start[1]] = { -1, -1 };

        int directions[4][2] = {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1}
        };

        while (!pq.empty()) {
            Node_d current = pq.top();
            pq.pop();
            if (current.row < 0 || current.row >= GRID_ROWS || current.col < 0 || current.col >= GRID_COLS) {
                continue;
            }
            if (visited[current.row][current.col]) continue;

            visited[current.row][current.col] = true;
            if (current.row == end[0] && current.col == end[1]) {
                Queue<int> path;
                while (current.row != -1 && current.col != -1) {
                    path.enqueue(current.row, current.col);
                    auto parentNode = parent[current.row * GRID_COLS + current.col];
                    current = { parentNode.first, parentNode.second, 0 };
                }
                Queue<int> newPath;
                newPath.reverse(path.front, path.rear);
                return newPath;
            }

            for (int i = 0; i < 4; i++) {
                int newRow = current.row + directions[i][0];
                int newCol = current.col + directions[i][1];

                if (newRow >= 0 && newRow < GRID_ROWS && newCol >= 0 && newCol < GRID_COLS) {
                    if (!visited[newRow][newCol] && maze[newRow][newCol] != '#') {
                        pq.push({ newRow, newCol, current.distance + 1 });
                        parent[newRow * GRID_COLS + newCol] = { current.row, current.col };
                    }
                }
            }
        }
        return {};
    }


    void DrawMaze(sf::RenderWindow& window, char**& maze, int cellSize, Queue<int>& shortestPath, Player& player, EnemyQueue& E, BulletQueue& bullet, Treasure*& types) {
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                sf::RectangleShape wall(sf::Vector2f(cellSize, cellSize));
                wall.setPosition(j * cellSize, i * cellSize);
                if (maze[i][j] == '#') {
                    wall.setFillColor(sf::Color::White);
                }
                else if (maze[i][j] == 'E') {
                    wall.setFillColor(sf::Color::Green);
                }
                else if (maze[i][j] == 'X') {
                    wall.setFillColor(sf::Color::Red);
                }
                else if (maze[i][j] == ' ') {
                    wall.setFillColor(sf::Color::Black);
                }
                window.draw(wall);
            }
        }
        if (!shortestPath.is_empty()) {
            sf::RectangleShape path(sf::Vector2f(cellSize - 2, cellSize - 2));
            path.setFillColor(sf::Color::Blue);
            Node_Q<int>* temp = shortestPath.front;
            while (temp) {
                path.setPosition(temp->col * cellSize, temp->row * cellSize);
                window.draw(path);
                temp = temp->next;
            }
        }
        window.draw(player.sprite);
        if (player.isShield) {
            window.draw(player.shield);
        }

        EnemyNode* node = E.front;
        while (node) {
            window.draw(node->e->sprite);
            node = node->next;
        }
        drawBullet(bullet, window);
        for (int i = 0; i < 5; i++) {
            if (!types[i].collision)
                window.draw(types[i].sprite);
        }

        player.inventory.showInventory(window, player.inventory.root, 10, HEIGHT + 7.0f);
    }

    void drawBullet(BulletQueue& bullet, sf::RenderWindow& window) {
        BulletNode* temp = bullet.front;
        BulletNode* prev = nullptr;
        while (temp != nullptr) {
            window.draw(temp->fire.sprite);
            if (!bulletCollision(temp->fire.sprite)) {
                if (temp->fire.direction == "left") {
                    temp->fire.setPosition(temp->fire.sprite.getPosition().x - 1, temp->fire.sprite.getPosition().y);
                }
                else if (temp->fire.direction == "right") {
                    temp->fire.setPosition(temp->fire.sprite.getPosition().x + 1, temp->fire.sprite.getPosition().y);
                }
            }
            else {
                BulletNode* toDelete = temp;
                temp = temp->next;
                if (prev != nullptr) {
                    prev->next = temp;
                }
                else {
                    bullet.front = temp;
                }
                if (bullet.rear == toDelete) {
                    bullet.rear = prev;
                }
                delete toDelete;
                continue;
            }
            prev = temp;
            temp = temp->next;
        }
    }




    bool checkCollision(sf::Sprite& player, string direction, float& x, float& y) {
        sf::FloatRect playerBounds = player.getGlobalBounds();
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                if (maze[i][j] == '#') {
                    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                    wall.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                    sf::FloatRect wallBounds = wall.getGlobalBounds();
                    if (playerBounds.intersects(wallBounds)) {
                        return true;
                    }
                }
            }
        }
        x = player.getPosition().x;
        y = player.getPosition().y;
        return false;
    }

    bool bulletCollision(Sprite& fire) {
        sf::FloatRect bulletBounds = fire.getGlobalBounds();
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                if (maze[i][j] == '#') {
                    sf::RectangleShape wall(sf::Vector2f(10, 10));
                    wall.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                    sf::FloatRect wallBounds = wall.getGlobalBounds();
                    if (bulletBounds.intersects(wallBounds)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    EnemyQueue& move_enemy(Queue<int>& enemyPath, EnemyQueue& E, Clock& c) {
        int i = 0;
        int j = 0;
        if (enemyPath.front) {
            i = enemyPath.front->row;
            j = enemyPath.front->col;
        }
        enemyPath.dequeue();
        if (enemyPath.front) {
            if (c.getElapsedTime().asMilliseconds() >= 300) {
                EnemyNode* node = E.front;
                while (node) {
                    node->e->sprite.setPosition((float)(enemyPath.front->col * CELL_SIZE), (float)(enemyPath.front->row * CELL_SIZE));
                    node = node->next;
                }
                c.restart();
            }
        }
        return E;
    }

    bool check_walls(Sprite& e, Sprite& p, float y) {
        for (int i = e.getPosition().x; i >= p.getPosition().x; i -= CELL_SIZE) {
            if (p.getPosition().x <= e.getPosition().x) {
                if (maze[(int)y][i / CELL_SIZE] == '#') {
                    return true;
                }
            }
        }

        for (int i = e.getPosition().x; i < p.getPosition().x; i += CELL_SIZE) {
            if (p.getPosition().x >= e.getPosition().x) {
                if (maze[(int)y][i / CELL_SIZE] == '#') {
                    return true;
                }
            }
        }
        return false;
    }


    bool check_walls_between_player_position(Sprite& e, Sprite& p) {
        if (e.getPosition().y == p.getPosition().y) {
            float y = e.getPosition().y / CELL_SIZE;
            return check_walls(e, p, y);
        }
        return true;
    }

    string check_player_position(Sprite& e, Sprite& p) {
        if (p.getPosition().x < e.getPosition().x) {
            return "left";
        }
        else if (p.getPosition().x > e.getPosition().x) {
            return "right";
        }
    }

    void enemy_fire(Bullet*& fire, BulletQueue& bullet, EnemyQueue& E, Player& p, sf::RenderWindow& window) {
        EnemyNode* node = E.front;
        if (fireClock.getElapsedTime().asMilliseconds() >= 800) {
            while (node) {
                if (check_walls_between_player_position(node->e->sprite, p.sprite) == false) {
                    fire = new Bullet;
                    fire->setSize(20, 20);
                    if (check_player_position(node->e->sprite, p.sprite) == "left") {
                        fire->direction = "left";
                        fire->setPosition(node->e->sprite.getPosition().x - CELL_SIZE, node->e->sprite.getPosition().y);
                    }
                    else if (check_player_position(node->e->sprite, p.sprite) == "right") {
                        fire->direction = "right";
                        fire->setPosition(node->e->sprite.getPosition().x + CELL_SIZE, node->e->sprite.getPosition().y);
                    }
                    bullet.enqueue(*fire);
                    window.draw(fire->sprite);

                }
                node = node->next;
            }
            fireClock.restart();
        }
    }

    void checkTreasureCollision(Player& player, Treasure* types, int treasureCount) {
        sf::FloatRect playerBounds = player.sprite.getGlobalBounds();
        for (int i = 0; i < treasureCount; i++) {
            if (!types[i].collision) {
                sf::FloatRect treasureBounds = types[i].sprite.getGlobalBounds();
                if (playerBounds.intersects(treasureBounds)) {
                    types[i].collision = true;
                    player.inventory.insert(types[i]);
                    player.score += types[i].score;
                    score = player.score;
                    updateScore();
                    if (i == 0) {
                        player.isShield = true;
                    }
                    if (i == 2) {
                        player.hasKey = true;
                    }
                }
            }
        }
    }

    void player_attacked(EnemyQueue& E, Player& p) {
        EnemyNode* node = E.front;
        while (node) {
            bool deleteCurrent = false;
            if (p.sprite.getPosition().y == node->e->sprite.getPosition().y) {
                if ((p.sprite.getPosition().x >= node->e->sprite.getPosition().x && p.sprite.getPosition().x < node->e->sprite.getPosition().x + CELL_SIZE) ||
                    (p.sprite.getPosition().x <= node->e->sprite.getPosition().x && p.sprite.getPosition().x > node->e->sprite.getPosition().x - CELL_SIZE)) {
                    deleteCurrent = true;
                }
            }
            if (deleteCurrent) {
                p.inventory.remove(15);
                EnemyNode* temp = node;
                node = node->next;
                E.deleteNode(temp);
            }
            else {
                node = node->next;
            }
        }
    }

    void updateScore() {
        scoreText.setString("Score: " + std::to_string(score));
    }

    bool player_bullet_collision(BulletQueue& bullet, Player& p) {
        BulletNode* curr = bullet.front;
        BulletNode* prev = nullptr;
        sf::FloatRect playerBounds = p.sprite.getGlobalBounds();
        while (curr) {
            sf::FloatRect bulletBounds = curr->fire.sprite.getGlobalBounds();
            if (bulletBounds.intersects(playerBounds)) {
                if (prev)
                    prev->next = curr->next;
                bullet.deleteNode(curr);
                curr = nullptr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void start_game() {
        srand(time(0));
        RenderWindow window(VideoMode(WIDTH, HEIGHT + 40), "Labyrinth Adventurer");
        GenerateMaze();
        Clock c;
        Queue<int> shortestPath;
        Queue<int> enemyPath;
        Player player("img/player_knight.png");
        player.setSize(15, 20);
        player.setPosition(CELL_SIZE, CELL_SIZE);
        Enemy* e1 = new Enemy("img/goblin1.png");
        e1->setSize(20, 20);
        EnemyQueue E;
        E.enqueue(e1);
        Treasure* types = new Treasure[5]{ {"img/shield.png",10},{"img/sword.png",15},{"img/key.png",20},{"img/potion_1.png",5},{"img/potion_1.png",5} };

        player.inventory.insert(types[1]);

        for (int i = 0; i < 5; i++) {
            while (true) {
                int x = rand() % GRID_ROWS;

                int y = rand() % GRID_COLS;
                while (y > GRID_COLS / 1.5) {
                    y = rand() % GRID_COLS;
                }
                if (maze[x][y] == ' ') {
                    types[i].setPosition(y * CELL_SIZE, x * CELL_SIZE);
                    types[i].setSize(21, 20);
                    break;
                }
            }
        }

        while (true) {
            int x = rand() % GRID_ROWS;
            int y = rand() % GRID_COLS;
            if (maze[x][y] == ' ') {
                e1->setPosition(y * CELL_SIZE, x * CELL_SIZE);
                break;
            }
        }
        float x = 0;
        float y = 0;
        bool gameOver=false;
        BulletQueue bullet;
        Bullet* fire = nullptr;
        while (window.isOpen()) {
            handleEvents(window);
            if (isPaused) {
                window.draw(pauseSprite);
                window.display();
                continue;
            }
            if (gameOver) {
                sf::Texture gameOverTexture;
                sf::Sprite gameOverSprite;
                if (gameOverTexture.loadFromFile("img/you_win.png")) {
                    gameOverSprite.setTexture(gameOverTexture);
                    float scaleFactorX = static_cast<float>(WIDTH) / gameOverTexture.getSize().x;
                    float scaleFactorY = static_cast<float>(HEIGHT) / gameOverTexture.getSize().y;
                    gameOverSprite.setScale(scaleFactorX, scaleFactorY);
                    window.draw(gameOverSprite);
                    window.display();
                }
                // Handle return to main menu
                while (window.isOpen()) {
                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();
                        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                            // Return to main menu
                            return;
                        }
                    }
                }
            }
            // Check for game-over condition
            if (player.score < 0) {
                // Display game-over screen
                sf::Texture gameOverTexture;
                sf::Sprite gameOverSprite;
                if (gameOverTexture.loadFromFile("img/game_over.png")) {
                    gameOverSprite.setTexture(gameOverTexture);
                    float scaleFactorX = static_cast<float>(WIDTH) / gameOverTexture.getSize().x;
                    float scaleFactorY = static_cast<float>(HEIGHT) / gameOverTexture.getSize().y;
                    gameOverSprite.setScale(scaleFactorX, scaleFactorY);
                    window.draw(gameOverSprite);
                    window.display();
                }
                // Handle return to main menu
                while (window.isOpen()) {
                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();
                        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                            // Return to main menu
                            return;
                        }
                    }
                }
            }

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                if (!checkCollision(player.sprite, "Up", x, y)) {
                    player.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 1);
                }
                else {
                    player.setPosition(x, y);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::A)) {
                if (!checkCollision(player.sprite, "Left", x, y)) {
                    player.setPosition(player.sprite.getPosition().x - 1, player.sprite.getPosition().y);
                }
                else {
                    player.setPosition(x, y);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::S)) {
                if (!checkCollision(player.sprite, "Down", x, y)) {
                    player.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y + 1);
                }
                else {
                    player.setPosition(x, y);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D)) {
                if (!checkCollision(player.sprite, "Right", x, y)) {
                    player.setPosition(player.sprite.getPosition().x + 1, player.sprite.getPosition().y);
                }
                else {
                    player.setPosition(x, y);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::F)) {
                if (player.inventory.search(15)) {
                    player_attacked(E, player);
                }
            }
            enemy_fire(fire, bullet, E, player, window);
            if (player_bullet_collision(bullet, player) && !player.isShield) {
                player.score -= 5;
                score = player.score;
                updateScore();
            }
            checkTreasureCollision(player, types, 5);
            EnemyNode* node = E.front;
            while (node) {
                int* Estart = new int[2] {(int)(node->e->sprite.getPosition().y / CELL_SIZE), (int)(node->e->sprite.getPosition().x / CELL_SIZE)};
                int* Eend = new int[2] {(int)(player.sprite.getPosition().y / CELL_SIZE), (int)(player.sprite.getPosition().x / CELL_SIZE)};
                enemyPath = FindShortestPath(Estart, Eend);
                node = node->next;
            }
            node = E.front;
            while (node) {
                if (node->e->sprite.getPosition().y == player.sprite.getPosition().y) {
                    if ((player.sprite.getPosition().x >= node->e->sprite.getPosition().x && player.sprite.getPosition().x < node->e->sprite.getPosition().x + 5 * CELL_SIZE) ||
                        (player.sprite.getPosition().x <= node->e->sprite.getPosition().x && player.sprite.getPosition().x > node->e->sprite.getPosition().x - 5 * CELL_SIZE)) {
                        break;
                    }
                }
                E = move_enemy(enemyPath, E, c);
                node = node->next;
            }

            if (player.sprite.getPosition().x == EXIT_COL * CELL_SIZE &&
                player.sprite.getPosition().y == EXIT_ROW * CELL_SIZE) {
                gameOver = true;
            }


            window.clear(Color::Black);
            DrawMaze(window, maze, CELL_SIZE, shortestPath, player, E, bullet, types);
            updateScore();
            window.draw(scoreText);
            window.display();
            
        }
    }
};
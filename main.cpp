#include <SFML/Graphics.hpp>

bool isColliding(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Square");
    sf::RectangleShape square(sf::Vector2f(50.f, 50.f));
    square.setFillColor(sf::Color::Green);
    square.setPosition(375.f, 300.f);

    sf::RectangleShape rectangle(sf::Vector2f(200.f, 400.f));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition(300.f, 500.f);

    float velocityX = 0.0f;
    float velocityY = 0.0f;
    const float gravity = 1000.0f;
    const float jumpForce = -700.0f;
    bool isJumping = false;
    bool spacePressed = false;

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
         sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Track the Spacebar being released
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                spacePressed = false;
            }
        }

        velocityX = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocityX = -200.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocityX = 200.0f;
        }
        // Only jump if the Spacebar is newly pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!isJumping && !spacePressed) {
                velocityY = jumpForce;
                isJumping = true;
            }
            spacePressed = true;
        }

        deltaTime *= 2.5;
        velocityY += gravity * deltaTime;
        square.move(velocityX * deltaTime, velocityY * deltaTime);

        if (isColliding(square, rectangle)) {
            if (velocityY > 0 && square.getPosition().y + square.getSize().y <= rectangle.getPosition().y + 10) {
                square.setPosition(square.getPosition().x, rectangle.getPosition().y - square.getSize().y);
                velocityY = 0;
                isJumping = false;
            } else if (velocityY <= 0 && square.getPosition().y >= rectangle.getPosition().y + rectangle.getSize().y - 10) {
                square.setPosition(square.getPosition().x, rectangle.getPosition().y + rectangle.getSize().y);
                velocityY = 0;
            } else {
                if(velocityX > 0) {
                    square.setPosition(rectangle.getPosition().x - square.getSize().x, square.getPosition().y);
                } else if(velocityX < 0) {
                    square.setPosition(rectangle.getPosition().x + rectangle.getSize().x, square.getPosition().y);
                }
                velocityX = 0;
            }
        }

        if (square.getPosition().y + square.getSize().y > window.getSize().y) {
            square.setPosition(square.getPosition().x, window.getSize().y - square.getSize().y);
            velocityY = 0;
            isJumping = false;
        }

        if (square.getPosition().x < 0) {
            square.setPosition(0, square.getPosition().y);
        }
        if (square.getPosition().x + square.getSize().x > window.getSize().x) {
            square.setPosition(window.getSize().x - square.getSize().x, square.getPosition().y);
        }

        window.clear();
        window.draw(square);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}

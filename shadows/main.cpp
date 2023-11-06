#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Ray 
{
    float angle;
    float length;
    float endX;
    float endY;
};

class GameObject 
{
public:
    GameObject(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) 
    {
        rect = sf::RectangleShape(sf::Vector2f(width, height));
    }

    void render(sf::RenderWindow& window) 
    {
        rect.setPosition(x, y);
        rect.setFillColor(sf::Color(255,20,5,255));
        window.draw(rect);
    }

    inline std::vector<sf::Vector2f> getVertices() const 
    {
        std::vector<sf::Vector2f> vertices;
        vertices.emplace_back(x, y);
        vertices.emplace_back(x + width, y);
        vertices.emplace_back(x + width, y + height);
        vertices.emplace_back(x, y + height);
        return vertices;
    }

    inline void setPosition(sf::Vector2i pos)
    {
        this->x = pos.x;
        this->y = pos.y;
    }

private:
    float x, y, width, height;

public:
    sf::RectangleShape rect; // I didn't feel like making getters.
};

struct ShadowPacket
{
    std::vector<sf::Vector2f> vertices;
};

class LightSource 
{
public:
    LightSource(float x, float y, float radius, int rayCount) : x(x), y(y), radius(radius), rayCount(rayCount) 
    {
        calculateRays();
        dot.setRadius(radius);
        dot.setPosition(x - radius, y - radius); // Account for the radius of the light source.
        dot.setFillColor(sf::Color(0,25,255,125));
    }

    inline void setPosition(float x, float y) 
    {
        this->x = x;
        this->y = y;
        dot.setPosition(x - radius, y - radius);
        calculateRays();
    }

    inline void setRadius(float radius) 
    {
        this->radius = radius;
        dot.setRadius(radius);
        dot.setPosition(x - radius, y - radius);
        calculateRays();
    }

    inline void setRayCount(int rayCount) 
    {
        this->rayCount = rayCount;
        calculateRays();
    }

    void renderRays(sf::RenderWindow& window) 
    {
        window.draw(dot);

        for (const Ray& ray : rays) 
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y), sf::Color::Blue),
                sf::Vertex(sf::Vector2f(ray.endX, ray.endY), sf::Color::Green)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    inline std::vector<Ray> getRays() const 
    {
        return rays;
    }

    inline sf::Vector2f getPosition() const 
    {
        return sf::Vector2f(x, y);
    }

public:
    float x, y;
    float radius;
    int rayCount;
    std::vector<Ray> rays;
    sf::CircleShape dot;

    void calculateRays() 
    {
        rays.clear();

        for (int i = 0; i < rayCount; i++) 
        {
            float angle = 2 * 3.141 * i / rayCount;
            float endX = x + radius * std::cos(angle);
            float endY = y + radius * std::sin(angle); 
            rays.push_back({ angle, radius, endX, endY });
        }
    }
};

class ShadowMap 
{
public:
    ShadowMap(int width, int height) : shadowMapWidth(width), shadowMapHeight(height) {}

    void calculateShadows(const std::vector<GameObject>& objects, const sf::Vector2f& lightSource, const std::vector<Ray>& rays) 
    {
        m_shadowPacket.vertices.clear();

        for (const Ray& ray : rays) 
        {
            // Calculate shadows for each ray.
            ShadowVolume shadowVolume = calculateShadowVolume(objects, lightSource, ray);

            // Add the shadow volume to the shadow polygon.
            m_shadowPacket.vertices.insert(m_shadowPacket.vertices.end(), shadowVolume.begin(), shadowVolume.end());
        }
    }

    void render(sf::RenderWindow& window) 
    {
        // Vertex shadow information.
        sf::VertexArray shadowVertices(sf::TriangleFan);
        for (const sf::Vector2f& vertex : m_shadowPacket.vertices) 
        {
            shadowVertices.append(sf::Vertex(vertex, sf::Color::Black));
        }

        // Make sure the vertices form a closed shape.
        if (!m_shadowPacket.vertices.empty()) 
        {
            shadowVertices.append(m_shadowPacket.vertices.front());
        }

        window.draw(shadowVertices);
    }

private:
    int shadowMapWidth, shadowMapHeight;
    ShadowPacket m_shadowPacket;

    using ShadowVolume = std::vector<sf::Vector2f>;

    ShadowVolume calculateShadowVolume(const std::vector<GameObject>& objects, const sf::Vector2f& lightSource, const Ray& ray) 
    {
        ShadowVolume shadowVolume;
        sf::Vector2f rayEnd(ray.endX, ray.endY);

        for (const auto& object : objects) 
        {
            ShadowVolume shadowSegment;
            sf::Vector2f lastIntersection;

            const std::vector<sf::Vector2f> objectVertices = object.getVertices();
            for (size_t i = 0; i < objectVertices.size(); i++) 
            {
                const sf::Vector2f& p1 = objectVertices[i];
                const sf::Vector2f& p2 = objectVertices[(i + 1) % objectVertices.size()];

                sf::Vector2f intersection;
                if (rayIntersectsSegment(lightSource, rayEnd, p1, p2, intersection)) 
                {
                    shadowSegment.push_back(intersection);
                }
            }

            if (!shadowSegment.empty()) 
            {
                shadowSegment.push_back(rayEnd);
                shadowVolume.insert(shadowVolume.end(), shadowSegment.begin(), shadowSegment.end());
            }
        }

        return shadowVolume;
    }

    bool rayIntersectsSegment(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& start, const sf::Vector2f& end, sf::Vector2f& intersection) 
    {
        sf::Vector2f segDir = p2 - p1;
        sf::Vector2f lineDir = end - start;
        float det = segDir.x * lineDir.y - lineDir.x * segDir.y;

        if (std::abs(det) < 1e-5) 
        {
            return false;
        }

        float t = ((start.x - p1.x) * lineDir.y - (start.y - p1.y) * lineDir.x) / det;
        float u = ((start.x - p1.x) * segDir.y - (start.y - p1.y) * segDir.x) / det;

        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) 
        {
            intersection.x = p1.x + t * segDir.x;
            intersection.y = p1.y + t * segDir.y;
            return true;
        }

        return false;
    }
};

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Shadows");

    LightSource lightSource(200, 350, 200, 48);
    std::vector<GameObject> objects;
    objects.emplace_back(300, 301, 100, 50);

    ShadowMap shadowMap(800, 600);
    GameObject* selectedObject = nullptr;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse click.
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (auto& object : objects) 
                {
                    if (object.rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) 
                    {
                        selectedObject = &object;
                    }
                }
            }
            // Handle object movement when dragging.
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) 
            {
                selectedObject = nullptr;
            }
            if (selectedObject && event.type == sf::Event::MouseMoved) 
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                selectedObject->setPosition(mousePosition);
            }
        }

        window.clear(sf::Color::White);

        lightSource.renderRays(window);

        shadowMap.calculateShadows(objects, lightSource.getPosition(), lightSource.getRays());

        shadowMap.render(window);

        for (auto& object : objects) 
        {
            object.render(window);
        }

        window.display();
    }

    return 0;
}

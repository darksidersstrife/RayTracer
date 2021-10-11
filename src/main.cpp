#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <intrin.h>
#include <fstream>

const float pi = 3.1415926535897932385f;

#include "geometry/point.h"
#include "geometry/vector.h"


#include "geometry/ray.h"
#include "rendering/camera.h"
#include "geometry/sphere.h"
#include "scene/scene.h"
#include "scene/TriangleModel.h"
#include "light/point.h"
#include "rendering/render_pool.h"
#include "cornell_box.h"

std::mt19937 e2(std::random_device{}());

const size_t sample_count = 1;
const size_t diffuse_sample_count = 1;

const float epsilon = 1e-3f;
const size_t num_threads = 5;




int main()
{
	std::uniform_real_distribution<float> dist(-0.5, 0.5);

	const int width = 1280, height = 720;
	sf::RenderWindow window(sf::VideoMode(width, height), "Simple ray tracer");
	window.setMouseCursorVisible(false);

	sf::Texture texture;
	texture.create(width, height);
	sf::Sprite             sprite(texture);

	Camera camera;
	camera.position = {278, 273, -800};
	camera.left = -1.0f * camera.left;
	float aspect_ratio = (float)width / height;
	float field_of_view = 0.620249486 * 2;
	float field_of_view_tan = tan(field_of_view / 2.f);

	auto last = clock();
	size_t frame_num = 0;

	sf::Vector2i center(window.getPosition().x + width / 2, window.getPosition().y + height / 2);
	sf::Mouse::setPosition(center);

    float total_intensity = 3.978874e+08;
	PointLight pl1;
    pl1.spectral_intensity[0] = 1;
    pl1.spectral_intensity[1] = 0.524f;
    pl1.spectral_intensity[2] =  0.134f;
    //pl1.spectral_intensity.spectre.normalize();
    pl1.spectral_intensity.spectre *= total_intensity;
    pl1.position = {278, 548.7, 279.5};

	Scene<TriangleModel<Triangle<float, 3>>, PointLight> scene = load_scene();
	scene.lights.push_back(pl1);


	window.setActive(false);
	std::thread renderThread([&]() mutable {
		window.setActive();
		RayGenerator rg(camera, window.getSize().x, window.getSize().y, aspect_ratio, field_of_view_tan);
		RaySampler<std::remove_reference_t<decltype(scene)>> rs(&scene, 0.001);
		RenderThreadPool<std::remove_reference_t<decltype(scene)>> rtp(num_threads, window.getSize().x, window.getSize().y, &rs, &rg);
		while (window.isOpen())
		{
			rtp.start();
			rtp.wait_completion();


			/*auto comp = [](size_t i) { return [i](const auto& a, const auto& b) {return a[i] < b[i];};};
			auto minr = std::min_element(rtp.image.begin(), rtp.image.end(), comp(0))->operator[](0);
			auto maxr = std::max_element(rtp.image.begin(), rtp.image.end(), comp(0))->operator[](0);
			auto ming = std::min_element(rtp.image.begin(), rtp.image.end(), comp(1))->operator[](1);
			auto maxg = std::max_element(rtp.image.begin(), rtp.image.end(), comp(1))->operator[](1);
			auto minb = std::min_element(rtp.image.begin(), rtp.image.end(), comp(2))->operator[](2);
			auto maxb = std::max_element(rtp.image.begin(), rtp.image.end(), comp(2))->operator[](2);*/

            std::vector<sf::Uint8> im(4 * rtp.image.size().x * rtp.image.size().y);
            for (size_t y = 0; y < window.getSize().y; y++) {
                for (size_t x = 0; x < window.getSize().x; x++) {
                    auto i = y * window.getSize().x + x;
                    auto to_rgb = [](auto num) {
                        return num / (num + 500) * 255;
                    };
                    for (size_t j = 0; j < 3; ++j)
                        im[4*i + j] = to_rgb(rtp.image[y][x][j]);
                    im[4*i + 3] = 255;
                }
            }

            texture.update(im.data());
			window.draw(sprite);
			window.display();

			auto now = clock();
			std::cout << "frame " << frame_num << ' ' << window.getSize().x << ' ' << window.getSize().y << ' ' << float(now - last) / CLOCKS_PER_SEC << std::endl;
			last = now;
			++frame_num;
		}
		std::ofstream f("../image.txt");
		f << rtp.image.size().x << '\n' << rtp.image.size().y << '\n';
        for (int i = 0; i < 3; ++i) {
            for (size_t y = 0; y < rtp.image.size().y; ++y) {
                for (int x = 0; x < rtp.image.size().x; ++x) {
                    f << rtp.image[y][x][i] << ' ';
                }
                f << '\n';
            }
        }
	});

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				texture.create(event.size.width, event.size.height);
				sprite.setTexture(texture, true);
				aspect_ratio = (float)event.size.width / event.size.height;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::W)
				{
					camera.position += 0.05f * camera.direction;
				}
				else if (event.key.code == sf::Keyboard::Key::S)
				{
					camera.position -= 0.05f * camera.direction;
				}
				else if (event.key.code == sf::Keyboard::Key::A)
				{
					camera.position += 0.05f * camera.left;
				}
				else if (event.key.code == sf::Keyboard::Key::D)
				{
					camera.position -= 0.05f * camera.left;
				}
				else if (event.key.code == sf::Keyboard::Key::Space)
				{
					if (event.key.shift)
						camera.position -= 0.05f * camera.up;
					else
						camera.position += 0.05f * camera.up;
				}
			}
		}

		if (window.hasFocus())
		{
			auto cur_mouse_pos = sf::Mouse::getPosition();
			auto x_change = (cur_mouse_pos.x - center.x);
			auto y_change = (cur_mouse_pos.y - center.y);
			sf::Mouse::setPosition(center);
			camera.rotate(x_change * 0.001f, -y_change * 0.001f);
		}
	}

	renderThread.join();

	return 0;
}
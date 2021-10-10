#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <intrin.h>

const float pi = 3.1415926535897932385f;

#include "geometry/point.h"
#include "geometry/vector.h"


#include "geometry/ray.h"
#include "scene/camera.h"
#include "geometry/sphere.h"
#include "scene/scene.h"
#include "scene/TriangleModel.h"
#include "light/point.h"


std::mt19937 e2(std::random_device{}());

const size_t sample_count = 1;
const size_t diffuse_sample_count = 1;

const float epsilon = 1e-3f;
const size_t num_threads = 5;

thread_local size_t thread_ray_count = 0;

std::atomic_uint32_t ray_count = 0;
std::atomic_uint32_t collision_count = 0;

//struct HitRecord
//{
//	float t;
//	Vec3f normal;
//	Vec3f color;
//};
//
//void multiplyVector(Vec3f& vec, float r[3][3])
//{
//	float x, y, z;
//
//	x = r[0][0] * vec[0] + r[0][1] * vec[1] + r[0][2] * vec[2];
//	y = r[1][0] * vec[0] + r[1][1] * vec[1] + r[1][2] * vec[2];
//	z = r[2][0] * vec[0] + r[2][1] * vec[1] + r[2][2] * vec[2];
//
//	vec[0] = x;
//	vec[1] = y;
//	vec[2] = z;
//}
//
//void rotateAgainstVector(float angle, const Vec3f& axis, Vec3f& vec)
//{
//	auto c = cos(angle);
//	auto s = sin(angle);
//	float r[3][3];
//
//	r[0][0] = c + (1.f - c) * axis[0] * axis[0];
//	r[0][1] = (1.f - c) * axis[0] * axis[1] - s * axis[2];
//	r[0][2] = (1.f - c) * axis[0] * axis[2] + s * axis[1];
//	r[1][0] = (1.f - c) * axis[0] * axis[1] + s * axis[2];
//	r[1][1] = c + (1.f - c) * axis[1] * axis[1];
//	r[1][2] = (1.f - c) * axis[1] * axis[2] - s * axis[0];
//	r[2][0] = (1.f - c) * axis[0] * axis[2] - s * axis[1];
//	r[2][1] = (1.f - c) * axis[1] * axis[2] + s * axis[0];
//	r[2][2] = c + (1.f	 - c) * axis[2] * axis[2];
//
//	multiplyVector(vec, r);
//}
//
//
//
//HitRecord hit_sphere(const Ray3f<>& r, const Sphere3f<>& s)
//{
//	auto v = (r.origin - s.position);
//	auto c =  v * v - s.r * s.r;
//	auto b = r.direction * v;
//	//auto a = r.direction * r.direction;
//	auto a = 1.0f;
//
//	auto d = b * b - a * c;
//	if (d < 0.0f)
//		return { -1.0f , {}, {} };
//
//	auto t = (sqrt(d) - b) / a;
//	if (t < 0)
//		return { -1.0f, {}, {} };
//
//	auto normal = (v + t * r.direction );
//	auto l = normal.length();
//	normal *= 1.0f / l;
//
//
//
//	return { t, normal, Vec3f(0.5f, 0.2f, 0.6f) };
//}
//
//Vec3f trace_ray(const Ray3f<>& ray, const std::vector<Sphere3f<>>& spheres, size_t limit)
//{
//	if (limit == 0)
//		return {0.0, 0.0, 0.0};
//
//	thread_ray_count++;
//	HitRecord rec{ -1.f };
//
//	for (auto& s : spheres)
//	{
//		auto hit = hit_sphere(ray, s);
//		if ((hit.t < rec.t || rec.t == -1) && hit.t != -1.f && hit.t >= epsilon)
//			rec = hit;
//	}
//
//	if (rec.t != -1)
//	{
//		if (rec.t <1e-2f)
//		{
//			//std::cout << rec.t << std::endl;
//			++collision_count;
//		}
//		std::uniform_real_distribution<float> planar_dist(0.0f, 2 * pi);
//		std::uniform_real_distribution<float> phi_probability_dist;
//
//		Vec3f color(0.0f, 0.0f, 0.0f);
//		for (size_t i = 0; i < diffuse_sample_count; i++)
//		{
//
//
//			auto diffuse_direction = rec.normal;
//			Vec3f ort;
//
//			if (diffuse_direction[0] == 0)
//			{
//				ort = { 1.f, 0.f, 0.f };
//			}
//			else
//			{
//				auto coeff = Vec3f{ diffuse_direction[0], diffuse_direction[1], 0.f }.length();
//				ort = { -diffuse_direction[1] / coeff , diffuse_direction[0] / coeff, 0.f };
//			}
//
//			auto planar_angle = planar_dist(e2);
//			rotateAgainstVector(planar_angle, diffuse_direction, ort);
//
//			auto phi = asin(sqrt(phi_probability_dist(e2)));
//			rotateAgainstVector(phi, ort, diffuse_direction);
//
//			color += 0.5f * trace_ray({ ray.origin + rec.t * ray.direction, diffuse_direction }, spheres, limit - 1);
//		}
//		color *= 1.0f / diffuse_sample_count;
//
//		return Vec3f(rec.color[0] * color[0], rec.color[1] * color[1], rec.color[2] * color[2]);
//	}
//	else
//	{
//		auto t = 0.5f * (ray.direction[0] + 1.0f);
//		return (1.0f - t) * Vec3f(1.0f, 1.0f, 1.0f) + t * Vec3f(0.5f, 0.7f, 1.0f);
//	}
//
//}

int main()
{
	std::uniform_real_distribution<float> dist(-0.5, 0.5);

	const int width = 1000, height = 600;
	sf::RenderWindow window(sf::VideoMode(width, height), "Simple ray tracer");
	window.setMouseCursorVisible(false);

	sf::Texture texture;
	texture.create(width, height);
	sf::Sprite             sprite(texture);
	std::vector<sf::Color> sfml_image(width * height);
	std::vector<spectral>  spectral_image(width * height);

	Camera camera;
	float aspect_ratio = (float)width / height;
	float field_of_view = pi / 2.f;
	float field_of_view_tan = tan(field_of_view / 2.f);

	auto last = clock();
	size_t frame_num = 0;

	sf::Vector2i center(window.getPosition().x + width / 2, window.getPosition().y + height / 2);
	sf::Mouse::setPosition(center);

	TriangleModel<Triangle<float, 3>> lside;
	lside.triangles.push_back({{-1.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 2.0f}});
	lside.triangles.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 2.0f}, {-1.0f, 0.0f, 2.0f}});
    lside.material.d.kd[0] = 1;
    lside.material.d.kd[1] = 0;
    lside.material.d.kd[2] = 0;
    lside.material.s.ks[0] = 0;
    lside.material.s.ks[1] = 0;
    lside.material.s.ks[2] = 0;
    
    TriangleModel<Triangle<float, 3>> rside;
    rside.triangles.push_back({ {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f},{1.0f, 0.0f, 2.0f}});
    rside.triangles.push_back({ {1.0f, 1.0f, 2.0f},{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 2.0f}});
    rside.material.d.kd[0] = 0;
    rside.material.d.kd[1] = 1;
    rside.material.d.kd[2] = 0;
    rside.material.s.ks[0] = 0;
    rside.material.s.ks[1] = 0;
    rside.material.s.ks[2] = 0;
    
    TriangleModel<Triangle<float, 3>> bside;
    bside.triangles.push_back({ {-1.0f, 0.0f, 2.0f},{-1.0f, 1.0f, 2.0f},{1.0f, 0.0f, 2.0f}});
    bside.triangles.push_back({ {-1.0f, 1.0f, 2.0f},{1.0f, 1.0f, 2.0f},{1.0f, 0.0f, 2.0f}});
    bside.material.d.kd[0] = 1;
    bside.material.d.kd[1] = 1;
    bside.material.d.kd[2] = 1;
    bside.material.s.ks[0] = 0;
    bside.material.s.ks[1] = 0;
    bside.material.s.ks[2] = 0;
    
    TriangleModel<Triangle<float, 3>> fside;
    fside.triangles.push_back({{-1.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 2.0f},  {1.0f, 0.0f, 1.0f}});
    fside.triangles.push_back({{1.0f, 0.0f, 1.0f},{-1.0f, 0.0f, 2.0f},  {1.0f, 0.0f, 2.0f}});
    fside.material.d.kd[0] = 1;
    fside.material.d.kd[1] = 1;
    fside.material.d.kd[2] = 1;
    fside.material.s.ks[0] = 0;
    fside.material.s.ks[1] = 0;
    fside.material.s.ks[2] = 0;
    
    TriangleModel<Triangle<float, 3>> cside;
    cside.triangles.push_back({{-1.0f, 1.0f, 2.0f}, {-1.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 1.0f}});
    cside.triangles.push_back({{-1.0f, 1.0f, 2.0f},{1.0f, 1.0f, 1.0f},  {1.0f, 1.0f, 2.0f}});
    cside.material.d.kd[0] = 1;
    cside.material.d.kd[1] = 1;
    cside.material.d.kd[2] = 1;
    cside.material.s.ks[0] = 0;
    cside.material.s.ks[1] = 0;
    cside.material.s.ks[2] = 0;
    
    TriangleModel<Triangle<float, 3>> box;
    box.triangles.push_back({{-0.25f, 0.25f, 1.75f},{0.25f, 0.25f, 1.75f},{0.0f, 0.25f, 1.25f}});
    box.triangles.push_back({{0.25f, 0.15f, 1.75f},{-0.25f, 0.15f, 1.75f},{0.0f, 0.15f, 1.25f}});
    box.triangles.push_back({{-0.25f, 0.15f, 1.75f},{0.25f, 0.15f, 1.75f},{0.25f, 0.25f, 1.75f}});
    box.triangles.push_back({{-0.25f, 0.15f, 1.75f},{0.25f, 0.25f, 1.75f},{-0.25f, 0.25f, 1.75f}});
    box.triangles.push_back({{0.0f, 0.25f, 1.25f},{-0.25f, 0.15f, 1.75f},{-0.25f, 0.25f, 1.75f}});
    box.triangles.push_back({{0.0f, 0.15f, 1.25f},{-0.25f, 0.15f, 1.75f},{0.0f, 0.25f, 1.25f}});
    box.triangles.push_back({{0.25f, 0.15f, 1.75f},{0.0f, 0.25f, 1.25f},{0.25f, 0.25f, 1.75f}});
    box.triangles.push_back({{0.25f, 0.15f, 1.75f},{0.0f, 0.15f, 1.25f},{0.0f, 0.25f, 1.25f}});
    box.material.d.kd[0] = 1;
    box.material.d.kd[1] = 0;
    box.material.d.kd[2] = 1;
    box.material.s.ks[0] = 0;
    box.material.s.ks[1] = 0;
    box.material.s.ks[2] = 0;
    
	PointLight pl1;
    pl1.spectral_intensity[0] = 1;
    pl1.spectral_intensity[1] = 1;
    pl1.spectral_intensity[2] = 1;
    pl1.position = {0.0, 0.8, 1.8};
    
    PointLight pl2;
    pl2.spectral_intensity[0] = 1.5;
    pl2.spectral_intensity[1] = 0.5;
    pl2.spectral_intensity[2] = 0.5;
    pl2.position = {0.5, 0.4, 1.5};

	Scene<TriangleModel<Triangle<float, 3>>, PointLight> scene;
	scene.objects.emplace_back(lside);
	scene.objects.emplace_back(rside);
	scene.objects.emplace_back(bside);
	scene.objects.emplace_back(fside);
	scene.objects.emplace_back(cside);
	scene.objects.emplace_back(box);
	scene.lights.push_back(pl1);
	scene.lights.push_back(pl2);


	window.setActive(false);
	std::thread renderThread([&]() mutable {
		window.setActive();
		while (window.isOpen())
		{
			Vec3f x_step = -(1.0f / window.getSize().x) * aspect_ratio * field_of_view_tan * camera.left;
			Vec3f y_step = -(1.0f / window.getSize().y) * field_of_view_tan * camera.up;

			Vec3f left_up_corner = camera.direction +
				0.5f * aspect_ratio * field_of_view_tan * camera.left +
				0.5f * field_of_view_tan * camera.up;

			std::vector<std::thread> threads;
			threads.reserve(num_threads);
			auto l = [&, camera](size_t y_begin, size_t y_end) {
				thread_ray_count = 0;
				for (size_t y = y_begin; y < y_end; y++)
				{
					for (size_t x = 0; x < window.getSize().x; x++)
					{
//					    if (x == window.getSize().x / 2 && y == window.getSize().y / 2)
//                        {
//                            Ray3f<> ray(camera.position, (left_up_corner + (x + dist(e2)) * x_step + (y + dist(e2)) * y_step).get_normalized());
//                            std::cout << scene.calculate_color_debug(ray, 0.001f, 4)[0];
//                        }
						spectral color{};

						for (size_t i = 0; i < sample_count; i++)
						{
							Ray3f<> ray(camera.position, (left_up_corner + (x + dist(e2)) * x_step + (y + dist(e2)) * y_step).get_normalized());
							color = color + scene.calculate_color(ray, 0.001f);
						}

						color = color / sample_count;
                        //spectral_image[y * window.getSize().x + x] = color;
                        sfml_image[y * window.getSize().x + x] = sf::Color(255 * pow((color[0]) / (color[0] + 1), 1.0 / 1),
                                          255 *pow((color[1]) / (color[1] + 1), 1.0 / 1),
                                          255 * pow((color[2]) / (color[2] + 1), 1.0 / 1));
					}
				}
				ray_count += thread_ray_count;
			};
			for (size_t i = 0; i < num_threads - 1; i++)
			{
				threads.emplace_back(l, window.getSize().y / num_threads * i, window.getSize().y / num_threads * ( i + 1));
			}
			threads.emplace_back(l, window.getSize().y / num_threads * (num_threads - 1), window.getSize().y);

			for (auto& t : threads)
				t.join();
			auto comp = [](size_t i) { return [i](const auto& a, const auto& b) {return a[i] < b[i];};};
			auto minr = std::min_element(spectral_image.begin(), spectral_image.end(), comp(0))->operator[](0);
			auto maxr = std::max_element(spectral_image.begin(), spectral_image.end(), comp(0))->operator[](0);
			auto ming = std::min_element(spectral_image.begin(), spectral_image.end(), comp(1))->operator[](1);
			auto maxg = std::max_element(spectral_image.begin(), spectral_image.end(), comp(1))->operator[](1);
			auto minb = std::min_element(spectral_image.begin(), spectral_image.end(), comp(2))->operator[](2);
			auto maxb = std::max_element(spectral_image.begin(), spectral_image.end(), comp(2))->operator[](2);
//            for(int i = 0; i < sfml_image.size(); ++i) {
//                sfml_image[i] = sf::Color(255 * pow((spectral_image[i][0] - minr) / (maxr - minr), 1.0 / 3),
//                                          255 *pow((spectral_image[i][1] - ming) / (maxg - ming), 1.0 / 3),
//                                          255 * pow((spectral_image[i][2] - minb) / (maxb - minb), 1.0 / 3));
//            }

            std::vector<sf::Uint8> im(4 * sfml_image.size());
            for (size_t y = 0; y < window.getSize().y; y++) {
                for (size_t x = 0; x < window.getSize().x; x++) {
                    auto i = y * window.getSize().x + x;
                    im[4*i] = sfml_image[i].r;
                    im[4*i + 1] = sfml_image[i].g;
                    im[4*i + 2] = sfml_image[i].b;
                    im[4*i + 3] = sfml_image[i].a;
                }
            }
            texture.update(im.data());

			window.draw(sprite);
			window.display();

			auto now = clock();
			std::cout << "frame " << frame_num << ' ' << window.getSize().x << ' ' << window.getSize().y << ' ' << float(now - last) / CLOCKS_PER_SEC << ' ' << (float)ray_count / 1e6 << std::endl;
			collision_count = 0;
			ray_count = 0;
			last = now;
			++frame_num;
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
				sfml_image.resize(event.size.width * event.size.height);
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
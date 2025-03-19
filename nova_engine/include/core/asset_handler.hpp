#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <list>
#include <filesystem>

namespace NovaEngine {
    typedef struct asset_t {
        sf::Texture text;
        sf::Sprite spr;

        asset_t(std::string path) {
            text.loadFromFile(path);
            spr.setTexture(text);
        }
        void draw(sf::RenderWindow& window) {
            window.draw(spr);
        }
        void draw(sf::RenderWindow& window, sf::IntRect rect) {
            sf::BlendMode blendMode = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add);
            spr.setTextureRect(rect);
            window.draw(spr, sf::RenderStates(blendMode));
        }
        void draw(sf::RenderWindow& window, sf::IntRect rect, sf::Vector2f scale) {
            sf::BlendMode blendMode = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add);
            spr.setTextureRect(rect);
            spr.setScale(scale);
            window.draw(spr, sf::RenderStates(blendMode));
        }
    } asset_t;

    typedef struct sprite_frame_t {
        std::vector<std::unique_ptr<asset_t>> gifs;
        int _index;
        double _frame;
        bool once;
        sf::Clock clk;
        sf::Time time;

        sprite_frame_t(std::string asset_path, double frame, bool onc = false) {
            std::filesystem::path directory(asset_path);
            std::list<std::string> list;
            std::string filePath;
            once = onc;
            _frame = frame;
            if (!std::filesystem::exists(asset_path)) std::exit(84);
            for (const auto &entry : std::filesystem::directory_iterator(directory)) {
                list.push_back(entry.path().string());
            }
            list.sort();
            while (!list.empty()) {
                filePath = list.front();
                gifs.push_back(std::make_unique<asset_t>(filePath));
                list.pop_front();
            }
            _index = 0;
            clk.restart();
        }

        void update() {
            time = clk.getElapsedTime();
            if (time.asSeconds() > _frame) {
                _index++;
                if (_index >= gifs.size() && once == false)
                    _index = 0;
                clk.restart();
            }
        }

        void draw_basics(sf::RenderWindow& window, sf::Vector2f position = sf::Vector2f(0, 0)) {
            gifs[_index]->spr.setPosition(position);
            gifs[_index]->draw(window);
        }

        void draw(sf::RenderWindow& window, sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0)) {
            gifs[_index]->spr.setPosition(position);
            gifs[_index]->spr.setScale(size);
            gifs[_index]->draw(window);
        }

        void draw(sf::RenderWindow& window, sf::IntRect rect, sf::Vector2f position = sf::Vector2f(0, 0)) {
            gifs[_index]->spr.setTextureRect(rect);
            gifs[_index]->spr.setPosition(position);
            gifs[_index]->draw(window);
        }

        void draw(sf::RenderWindow& window, sf::IntRect rect, sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0)) {
            gifs[_index]->spr.setTextureRect(rect);
            gifs[_index]->spr.setPosition(position);
            gifs[_index]->spr.setScale(size);
            gifs[_index]->draw(window);
        }

        ~sprite_frame_t() {}
    } sprite_frame_t;

    typedef struct audio_tiles_t {
        sf::SoundBuffer buffer;
        sf::Sound sound;
        audio_tiles_t(const std::string path) {
            buffer.loadFromFile(path);
            sound.setBuffer(buffer);
        }
        void play() {
            sound.play();
        }
    } audio_tiles_t;

    class Asset_handler {
        public:
            Asset_handler() {}
            ~Asset_handler() {}
            void set_asset(std::string asset_name, const std::string path) {
                assets[asset_name] = std::make_unique<asset_t>(path);
            }
            void set_multi_frame_asset(std::string asset_name, const std::string path, double frame, bool once = false) {
                multi_assets[asset_name] = std::make_unique<sprite_frame_t>(path, frame, once);
            }
            void set_font(std::string font_name, const std::string path) {
                fonts[font_name] = std::make_unique<sf::Font>();
                fonts_path[font_name] = path;
                if (!fonts[font_name]->loadFromFile(path)) {
                    std::cout << "Couldn't load" << std::endl;
                }
            }
            std::string get_font_path(std::string font_id) {
                if (fonts_path.find(font_id) != fonts_path.end())
                    return fonts_path[font_id];
                return "";
            }
            void set_audio(std::string audio_name, const std::string path) {
                audio_tiles[audio_name] = std::make_unique<audio_tiles_t>(path);
            }
            void set_music_file(std::string music_name, std::string path) {
                music[music_name] = std::make_unique<sf::Music>();
                music[music_name]->openFromFile(path);
            }
            void set_font(std::string font_name, sf::Text &text) {
                if (fonts.find(font_name) != fonts.end())
                    text.setFont(*fonts[font_name]);
                else
                    throw std::runtime_error("Font not found");
            }
            sf::Music &get_music(std::string music_name) {
                if (music.find(music_name)!= music.end())
                    return *music[music_name];
                else
                    throw std::runtime_error("Music not found");
            }
            void draw_asset(sf::RenderWindow& window, std::string asset_name, sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0)) {
                if (assets.find(asset_name) != assets.end()) {
                    assets[asset_name]->spr.setPosition(position);
                    assets[asset_name]->spr.setScale(size);
                    assets[asset_name]->draw(window);
                } else if (multi_assets.find(asset_name) != multi_assets.end()) {
                    multi_assets[asset_name]->draw(window, size, position);
                    multi_assets[asset_name]->update();
                }
            }
            void draw_asset(sf::RenderWindow& window, std::string asset_name, sf::IntRect &rect, sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0)) {
                if (assets.find(asset_name) != assets.end()) {
                    assets[asset_name]->spr.setPosition(position);
                    assets[asset_name]->draw(window, rect, size);
                } else if (multi_assets.find(asset_name) != multi_assets.end()) {
                    multi_assets[asset_name]->draw(window, rect, size, position);
                    multi_assets[asset_name]->update();
                }
            }
            void play_music(std::string music_name, int volume, bool loop) {
                if (music.find(music_name)!= music.end() && music[music_name]->getStatus() == sf::Music::Stopped) {
                    music[music_name]->setVolume(volume);
                    music[music_name]->setLoop(loop);
                    music[music_name]->play();
                }
            }
            void play_audio(std::string music_name, int volume, bool loop) {
                if (audio_tiles.find(music_name)!= audio_tiles.end()) {
                    audio_tiles[music_name]->play();
                }
            }
            void stop_music(std::string music_name) {
                if (music.find(music_name)!= music.end()) {
                    music[music_name]->stop();
                }
            }
            void stop_audio(std::string music_name) {
                if (audio_tiles.find(music_name)!= audio_tiles.end()) {
                    audio_tiles[music_name]->sound.stop();
                }
            }
            void set_volume(int volume) {
                for (auto &it : music) {
                    it.second->setVolume(volume);
                }
                for (auto &it : audio_tiles) {
                    it.second->sound.setVolume(volume);
                }
            }

        private:
            std::map<std::string, std::unique_ptr<asset_t>> assets;
            std::map<std::string, std::unique_ptr<sprite_frame_t>> multi_assets;
            std::map<std::string, std::unique_ptr<sf::Font>> fonts;
            std::map<std::string, std::string> fonts_path;
            std::map<std::string, std::unique_ptr<audio_tiles_t>> audio_tiles;
            std::map<std::string, std::unique_ptr<sf::Music>> music;
    };
}

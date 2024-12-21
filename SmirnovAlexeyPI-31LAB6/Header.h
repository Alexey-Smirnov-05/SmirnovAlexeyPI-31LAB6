#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <vector>

using namespace std;

// Абстрактный класс для медиа
class Media {
public:
    virtual void playSong() = 0; // Чисто виртуальная функция
    virtual ~Media() {} // Виртуальный деструктор
};

// Класс для хранения информации о треке
class Track : public Media { // Наследование от Media
private:
    std::string title;
public:
    Track() {}
    Track(const std::string& title) : title(title) {}

    std::string getTitle() const {
        return title;
    }

    void setTitle(const std::string& title) {
        this->title = title;
    }

    // Перегрузка оператора ==
    bool operator==(const Track& other) const {
        return this->title == other.title;
    }

    // Дружественная функция для вывода информации о треке
    friend ostream& operator<<(ostream& os, const Track& track) {
        os << track.title;
        return os;
    }

    void playSong() override { // Реализация виртуальной функции
        cout << "Играет трек: " << title << endl;
    }
};


// Класс для хранения плейлиста
class Playlist : public Media { // Наследование от Media
protected: // Модификатор protected
    std::vector<Track> tracks; // Используем std::vector вместо массива
    int current_track;
    static int instance_count; // Статическое поле
public:
    Playlist() : current_track(0) {
        instance_count++;
    }

    Playlist(const Playlist& other) : current_track(other.current_track), tracks(other.tracks) {
        instance_count++;
    }

    Playlist& operator=(const Playlist& other) {
        if (this != &other) {
            current_track = other.current_track;
            tracks = other.tracks;
        }
        return *this;
    }

    virtual void playSong() override { // Виртуальная функция
        if (!tracks.empty()) {
            cout << "Сейчас играет: " << tracks[current_track].getTitle() << endl;
        }
        else {
            cout << "Плейлист пуст." << endl;
        }
    }


    ~Playlist() {
        instance_count--;
    }

    static int getInstanceCount() { // Статический метод
        return instance_count;
    }

    void viewSongs() {
        for (size_t i = 0; i < tracks.size(); i++) {
            cout << i + 1 << ". " << tracks[i] << endl;
        }
    }

    void addSong(const Track& song) {
        tracks.push_back(song);
    }

    void removeSong(int index) {
        if (index >= 0 && index < tracks.size()) {
            tracks.erase(tracks.begin() + index);
        }
    }

    void getSongByIndex(int index, Track*& song) { // Возврат через указатель
        if (index >= 0 && index < tracks.size()) {
            song = &tracks[index];
        }
        else {
            song = nullptr;
        }
    }

    void getSongByIndex(int index, Track& song) { // Возврат через ссылку
        if (index >= 0 && index < tracks.size()) {
            song = tracks[index];
        }
    }

    int getTotalNumberOfTracks() {
        return tracks.size();
    }

    std::vector<Track>& getTracks() {
        return tracks;
    }

    int getCurrentTrack() {
        return current_track;
    }

    void setCurrentTrack(int current_track) {
        this->current_track = current_track;
    }

    void load_tracks_from_file(const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            // Если файл не найден, создаем его
            std::ofstream newFile(filename); // Создаем новый пустой файл
            if (!newFile) {
                throw runtime_error("Ошибка создания файла");
            }
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            Track song;
            song.setTitle(line);
            addSong(song);
        }

        file.close();
    }

    void save_tracks_to_file(const char* filename) {
        std::ofstream file(filename);
        if (!file) {
            throw runtime_error("Ошибка открытия файла");
        }

        for (size_t i = 0; i < tracks.size(); i++) {
            file << tracks[i].getTitle() << '\n'; // Записываем строки в файл
        }

        file.close();
    }
};

// Новый производный класс
class AdvancedPlaylist : public Playlist {
private:
    string name;
public:
    AdvancedPlaylist() : Playlist(), name("MyPlaylist") {} // Вызов конструктора базового класса
    AdvancedPlaylist(const string& name) : Playlist(), name(name) {}
    AdvancedPlaylist(const AdvancedPlaylist& other) : Playlist(other), name(other.name) {}

    AdvancedPlaylist& operator=(const AdvancedPlaylist& other) {
        if (this != &other) {
            Playlist::operator=(other);
            name = other.name;
        }
        return *this;
    }

    void playSong() override { // Переопределение виртуальной функции
        if (!tracks.empty()) {
            cout << "Играет из AdvancedPlaylist '" << name << "': " << tracks[current_track].getTitle() << endl;
        }
        else {
            cout << "Плейлист '" << name << "' пуст." << endl;
        }
    }

    void playSpecificSong(int index) {
        if (index >= 0 && index < tracks.size()) {
            cout << "Играет конкретный трек: " << tracks[index].getTitle() << endl;
        }
        else {
            cout << "Неверный индекс трека." << endl;
        }
    }

    void setName(const string& name) {
        this->name = name;
    }

    string getName() const {
        return name;
    }
};

int Playlist::instance_count = 0; // Инициализация статического поля

// Класс для демонстрации признака ассоциаций
class User {
private:
    Playlist* playlist;
public:
    User(Playlist* playlist) : playlist(playlist) {}
    void addSongToPlaylist(const Track& song) {
        playlist->addSong(song);
    }
    void removeSongFromPlaylist(int index) {
        playlist->removeSong(index);
    }
    void viewPlaylist() {
        playlist->viewSongs();
    }
};


// Класс для хранения громкости
class Volume {
private:
    int volume;
public:
    Volume() : volume(50) {}
    void changeVolume(int new_volume) {
        if (new_volume >= 0 && new_volume <= 100) {
            volume = new_volume;
            cout << ("Громкость изменена на %d%%\n", volume);
        }
        else {
            cout << ("Неверная громкость\n");
        }
    }
};

// Класс для хранения воспроизведения
class Playback {
private:
    Playlist* playlist;
    int current_track;
public:
    Playback(Playlist* playlist, int current_track) : playlist(playlist), current_track(current_track) {}
    void playSong() {
        playlist->playSong(); // Вызов виртуальной функции из Playback
    }
};

// Класс для хранения паузы
class Pause {
private:
    Playlist* playlist;
public:
    Pause(Playlist* playlist) : playlist(playlist) {}
    void pauseSong() {
        cout << ("На паузе\n");
    }
};

// Класс для хранения перехода к следующему треку
class Skip {
private:
    Playlist* playlist;
public:
    Skip(Playlist* playlist) : playlist(playlist) {}
    void skipSong() {
        playlist->setCurrentTrack((playlist->getCurrentTrack() + 1) % playlist->getTotalNumberOfTracks());
        Playback playback(playlist, playlist->getCurrentTrack());
        playback.playSong();
    }
};

// Класс для хранения возврата к предыдущему треку
class Previous {
private:
    Playlist* playlist;
public:
    Previous(Playlist* playlist) : playlist(playlist) {}
    void previousSong() {
        playlist->setCurrentTrack((playlist->getCurrentTrack() - 1 + playlist->getTotalNumberOfTracks()) % playlist->getTotalNumberOfTracks());
        Playback playback(playlist, playlist->getCurrentTrack());
        playback.playSong();
    }
};

// Класс для хранения просмотра треков в плейлисте
class View {
private:
    Playlist* playlist;
public:
    View(Playlist* playlist) : playlist(playlist) {}
    void viewSongs() {
        if (playlist->getTotalNumberOfTracks() == 0) {
            cout << "Нет треков в плейлисте\n";
            return;
        }
        cout << "Список треков:\n";
        for (int i = 0; i < playlist->getTotalNumberOfTracks(); i++) {
            cout << i + 1 << ". " << playlist->getTracks()[i].getTitle() << endl;
        }
    }
};
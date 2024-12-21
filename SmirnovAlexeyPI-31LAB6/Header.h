#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

// ����������� ����� ��� �����
class Media {
public:
    virtual void playSong() = 0; // ����� ����������� �������
};

// ����� ��� �������� ���������� � �����
class Track : public Media { // ������������ �� Media
private:
    std::string title; // ���������� std::string ������ char[]
public:
    Track() {}
    Track(const std::string& title) : title(title) {}

    std::string getTitle() const {
        return title;
    }

    void setTitle(const std::string& title) {
        this->title = title;
    }

    // ���������� ��������� ==
    bool operator==(const Track& other) const {
        return this->title == other.title;
    }

    // ������������� ������� ��� ������ ���������� � �����
    friend ostream& operator<<(ostream& os, const Track& track) {
        os << track.title;
        return os;
    }

    void playSong() override { // ���������� ����������� �������
        cout << "������ ����: " << title << endl;
    }
};


// ����� ��� �������� ���������
class Playlist : public Media { // ������������ �� Media
protected: // ����������� protected
    Track* tracks;
    int total_number_of_tracks;
    int current_track;
    static int instance_count; // ����������� ����
public:
    Playlist() : total_number_of_tracks(0), current_track(0) {
        tracks = new Track[1];
        instance_count++;
    }

    // ����������� �����
    Playlist(const Playlist& other) {
        total_number_of_tracks = other.total_number_of_tracks;
        current_track = other.current_track;

        // ������� ����� ������ � �������� ������
        tracks = new Track[total_number_of_tracks];
        for (int i = 0; i < total_number_of_tracks; i++) {
            tracks[i] = other.tracks[i];
        }
    }

    // ���������� ��������� ������������
    Playlist& operator=(const Playlist& other) {
        if (this != &other) {
            // ������� ������ ������
            delete[] tracks;

            total_number_of_tracks = other.total_number_of_tracks;
            current_track = other.current_track;

            // ������� ����� ������ � �������� ������
            tracks = new Track[total_number_of_tracks];
            for (int i = 0; i < total_number_of_tracks; i++) {
                tracks[i] = other.tracks[i];
            }
        }
        return *this;
    }

    virtual void playSong() override { // ����������� �������
        cout << "������ ������: " << tracks[current_track].getTitle() << endl;
    }

    ~Playlist() {
        delete[] tracks;
        instance_count--;
    }

    static int getInstanceCount() { // ����������� �����
        return instance_count;
    }

    void viewSongs() {
        for (int i = 0; i < total_number_of_tracks; i++) {
            cout << i + 1 << ". " << tracks[i] << endl;
        }
    }

    void addSong(const Track& song) {
        Track* newTracks = new Track[total_number_of_tracks + 1];
        for (int i = 0; i < total_number_of_tracks; i++) {
            newTracks[i] = tracks[i];
        }
        newTracks[total_number_of_tracks] = song;
        delete[] tracks;
        tracks = newTracks;
        total_number_of_tracks++;
    }

    void removeSong(int index) {
        for (int i = index; i < total_number_of_tracks - 1; i++) {
            tracks[i] = tracks[i + 1];
        }
        total_number_of_tracks--;
        Track* newTracks = new Track[total_number_of_tracks];
        for (int i = 0; i < total_number_of_tracks; i++) {
            newTracks[i] = tracks[i];
        }
        delete[] tracks;
        tracks = newTracks;
    }

    void getSongByIndex(int index, Track*& song) { // ������� ����� ���������
        if (index >= 0 && index < total_number_of_tracks) {
            song = &tracks[index];
        }
        else {
            song = nullptr;
        }
    }

    void getSongByIndex(int index, Track& song) { // ������� ����� ������
        if (index >= 0 && index < total_number_of_tracks) {
            song = tracks[index];
        }
    }

    int getTotalNumberOfTracks() {
        return total_number_of_tracks;
    }

    Track* getTracks() {
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
            // ���� ���� �� ������, ������� ���
            std::ofstream newFile(filename); // ������� ����� ������ ����
            if (!newFile) {
                throw runtime_error("������ �������� �����");
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
            throw runtime_error("������ �������� �����");
        }

        for (int i = 0; i < total_number_of_tracks; i++) {
            file << tracks[i].getTitle() << '\n'; // ���������� ������ � ����
        }

        file.close();
    }
};

int Playlist::instance_count = 0; // ������������� ������������ ����

// ����� ��� ������������ �������� ����������
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


// ����� ��� �������� ���������
class Volume {
private:
    int volume;
public:
    Volume() : volume(50) {}
    void changeVolume(int new_volume) {
        if (new_volume >= 0 && new_volume <= 100) {
            volume = new_volume;
            cout << ("��������� �������� �� %d%%\n", volume);
        }
        else {
            cout << ("�������� ���������\n");
        }
    }
};

// ����� ��� �������� ���������������
class Playback {
private:
    Playlist* playlist;
    int current_track;
public:
    Playback(Playlist* playlist, int current_track) : playlist(playlist), current_track(current_track) {}
    void playSong() {
        cout << "������ ������: " << playlist->getTracks()[current_track].getTitle() << endl;
    }
};

// ����� ��� �������� �����
class Pause {
private:
    Playlist* playlist;
public:
    Pause(Playlist* playlist) : playlist(playlist) {}
    void pauseSong() {
        cout << ("�� �����\n");
    }
};

// ����� ��� �������� �������� � ���������� �����
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

// ����� ��� �������� �������� � ����������� �����
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

// ����� ��� �������� ��������� ������ � ���������
class View {
private:
    Playlist* playlist;
public:
    View(Playlist* playlist) : playlist(playlist) {}
    void viewSongs() {
        if (playlist->getTotalNumberOfTracks() == 0) {
            cout << "��� ������ � ���������\n";
            return;
        }
        cout << "������ ������:\n";
        for (int i = 0; i < playlist->getTotalNumberOfTracks(); i++) {
            cout << i + 1 << ". " << playlist->getTracks()[i].getTitle() << endl;
        }
    }
};
#include "Header.h"

int main() {
    system("chcp 1251");
    setlocale(LC_ALL, "RUS");

    AdvancedPlaylist playlist; // Используем новый производный класс
    User user(&playlist);
    Volume volume;

    playlist.load_tracks_from_file("playlist.txt");

    int choice;
    while (1) {
        cout << ("Меню плеера:\n");
        cout << ("0. Добавить трек\n");
        cout << ("1. Изменить громкость\n");
        cout << ("2. Воспроизвести\n");
        cout << ("3. Пауза\n");
        cout << ("4. Следующий трек\n");
        cout << ("5. Предыдущий трек\n");
        cout << ("6. Удалить трек\n");
        cout << ("7. Просмотреть треки в плейлисте\n");
        cout << ("8. Выход\n");
        cout << ("Действие: ");
        scanf("%d", &choice);

        switch (choice) {
        case 0: {
            Track song;
            cout << "Введите название трека: ";
            std::string title;
            std::getline(cin >> std::ws, title); // Читаем строку с названием трека
            song.setTitle(title);
            user.addSongToPlaylist(song);
            playlist.save_tracks_to_file("playlist.txt");
            break;
        }
        case 1: {
            int new_volume;
            cout << ("Введите громкость (0-100): ");
            scanf("%d", &new_volume);
            volume.changeVolume(new_volume);
            break;
        }
        case 2:
            if (playlist.getTotalNumberOfTracks() > 0) {
                playlist.playSong(); // Вызов виртуальной функции
            }
            else {
                cout << "Нет треков в плейлисте\n";
            }
            break;
        case 3:
            if (playlist.getTotalNumberOfTracks() > 0) {
                Pause pause(&playlist);
                pause.pauseSong();
            }
            else {
                cout << ("Нет треков в пл ейлисте\n");
            }
            break;
        case 4:
            if (playlist.getTotalNumberOfTracks() > 0) {
                Skip skip(&playlist);
                skip.skipSong();
            }
            else {
                cout << "Нет треков в плейлисте\n";
            }
            break;
        case 5:
            if (playlist.getTotalNumberOfTracks() > 0) {
                Previous previous(&playlist);
                previous.previousSong();
            }
            else {
                cout << ("Нет треков в плейлисте\n");
            }
            break;
        case 6: {
            if (playlist.getTotalNumberOfTracks() > 0) {
                int index;
                cout << ("Введите индекс трека для удаления: ");
                scanf("%d", &index);
                if (index >= 1 && index <= playlist.getTotalNumberOfTracks()) {
                    playlist.removeSong(index - 1);
                    playlist.save_tracks_to_file("playlist.txt"); // Сохраняем изменения в файле
                }
                else {
                    cout << ("Неверный индекс\n");
                }
            }
            else {
                cout << ("Нет треков в плейлисте\n");
            }
            break;
        }
        case 7: {
            View view(&playlist);
            view.viewSongs();
            break;
        }
        case 8:
            return 0;
        default:
            cout << "Ошибка выбора действия\n";
        }
        cout << ("\n\n");
    }
    return 0;
}
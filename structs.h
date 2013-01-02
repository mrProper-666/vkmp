#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QUrl>

typedef struct {
    QString artist;
    QString song;
    QString album;
    int duration;
    QString url;
} playListItem;

typedef struct {
    QString client_id;              // id приложения
    QString scope;                  // настройки доступа
    QString redirect_uri;           // урл для редиректа
    QString display;                // тип отображения wap\popup\page\touch
    QString response_type;          // =token
} loginVars;

typedef struct {
    QString l;                      // id пользователя в ВКонтакте
    QString p;                      // "пароль"
    QString remixsid;               // id сессии, генерируется с помощью "l" и "p" (привязана к IP)
    bool save;                      // сохранять куки (false - если стоит галочка "Чужой компьютер")
} cookies;

typedef struct {
    QString access_token;           // access_token
    QString uid;                    // id пользователя
} sessionVars;

typedef struct {
    QString name;                   // имя пользователя
    QString id;                     // id пользователя
    QString psswd;                  // пароль
    QString remixsid;               // id сессии
    QString pic;                    // аватар
} loginMVars;

typedef struct {
    QString owner_id;               // id владельца
    QString id;                     // id альбома
    QString title;                  // название альбома
} albums;

typedef struct {
    QString aid;                    // id записи
    QString owner_id;               // id владельца
    QString artist;                 // артист
    QString title;                  // заголовок
    int duration;                   // продолжительность
    QUrl url;                       // ссылка
    QString album_id;               // id альбома
} audio;
#endif // STRUCTS_H

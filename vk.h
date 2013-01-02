#ifndef VK_H
#define VK_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QtWebKit/QWebView>
#include <QApplication>
#include <QtGui/QDesktopWidget>
#include <QNetworkCookie>
#include <QtWebKit/QWebFrame>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

#include "structs.h"
#include "settings.h"

class VK : public QObject
{
    Q_OBJECT
public:
    explicit VK(QObject *parent = 0);
    void authorize(const QString &l, const QString &p, const QString &remixsid);

    QHash<QString, albums> albumsHash;
    QMultiHash<QString, audio> audioHash;

private:
    loginVars s_loginVars;
    cookies s_cookies;
    sessionVars s_sessionVars;

    QUrl authURL;
    QNetworkAccessManager *http;
    QNetworkRequest request;
    QWebView *authView;
    Settings *Csettings;

    void userParse(QByteArray xml);
    void albumsParse(QByteArray xml);
    void audioParse(QByteArray xml);
    void getAlbums(QString access_token, QString uid);
    void getUserInfo(QString access_token, QString uid);
    void getAudio(QString access_token, QString uid);

signals:
    void connected();
    void userDone(loginMVars *);
    void albumsDone(QHash<QString, albums> *);
    void audioDone(QMultiHash<QString, audio>*);

public slots:
    void connectToVk();

private slots:
    void slotAuthPageLoaded(const bool &ok);
    void slotConnected();
    void slotResponse(QNetworkReply *reply);

};

#endif // VK_H

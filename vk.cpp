#include "vk.h"

#define app "2285023"                               //id приложения
#define red_url "http://api.vk.com/blank.html"      //redirect_uri
#define server "https://api.vk.com/method/"         //урл для запросов апи

VK::VK(QObject *parent) :
    QObject(parent)
{
    s_loginVars.client_id=app;
    s_loginVars.scope="friends,audio,wall,notify,messages";
    s_loginVars.redirect_uri=red_url;
    s_loginVars.display="popup";
    s_loginVars.response_type="token";

    //собираем урл для авторизации
    authURL = QString("http://oauth.vk.com/oauth/authorize?client_id=%1&scope=%2&redirect_uri=%3&display=%4&response_type=%5")
            .arg(s_loginVars.client_id)
            .arg(s_loginVars.scope)
            .arg(s_loginVars.redirect_uri)
            .arg(s_loginVars.display)
            .arg(s_loginVars.response_type);
}

void VK::connectToVk(){
    Settings::instance()->setUid(Settings::instance()->getLastUid().toString());
    s_cookies.l = Settings::instance()->loadProfile("net/id", "none").toString();
    s_cookies.p = Settings::instance()->loadProfile("net/psswd", "none").toString();
    s_cookies.remixsid = Settings::instance()->loadProfile("net/sid", "none").toString();
    authorize(s_cookies.l, s_cookies.p, s_cookies.remixsid);
}

void VK::authorize(const QString &l, const QString &p, const QString &remixsid){
    //объект для запросов api
    http = new QNetworkAccessManager(this);

    //создаем виджет браузера
    authView = new QWebView();
    connect(authView->page(),SIGNAL(loadFinished(bool)),this,SLOT(slotAuthPageLoaded(bool)));
    authView->setWindowFlags(Qt::CustomizeWindowHint);
    authView->resize(600, 430);
    authView->setWindowTitle(tr("vkMP | Authorization . . ."));
    authView->move((QApplication::desktop()->width() - authView->width())/2,
                      (QApplication::desktop()->height() - authView->height())/2);

    s_cookies.l = l;
    s_cookies.p = p;
    s_cookies.remixsid = remixsid;

    //устанавливаю cookies
        if (!s_cookies.p.isEmpty() || !s_cookies.remixsid.isEmpty()){
            QList<QNetworkCookie> listCookie;
            listCookie << QNetworkCookie::parseCookies(QByteArray().append("l="+s_cookies.l+"; path=/; domain=login.vk.com"))
                       << QNetworkCookie::parseCookies(QByteArray().append("p="+s_cookies.p+"; path=/; domain=login.vk.com"))
                       << QNetworkCookie::parseCookies(QByteArray().append("s=1; path=/; domain=login.vk.com"));
            authView->page()->networkAccessManager()->cookieJar()->setCookiesFromUrl(listCookie, QUrl("http://login.vk.com"));
            listCookie.clear();
            if (s_cookies.remixsid.isEmpty())
                s_cookies.remixsid = "none";
            listCookie << QNetworkCookie::parseCookies(QByteArray().append("remixsid="+s_cookies.remixsid+"; path=/; domain=."+authURL.host()));
            authView->page()->networkAccessManager()->cookieJar()->setCookiesFromUrl(listCookie, authURL);
        }


    //запрос
    authView->load(QUrl(authURL));
    connect(this, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(http, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotResponse(QNetworkReply*)));

}

void VK::slotAuthPageLoaded(const bool &ok){
    QString S = authView->url().toEncoded();
    //qDebug() << S;

    //выдергиваем куки

    s_cookies.remixsid.clear();
    const QNetworkCookieJar *cookie = authView->page()->networkAccessManager()->cookieJar();
    for (int i=0; i < cookie->cookiesForUrl(authURL).count(); i++)
        if (cookie->cookiesForUrl(authURL).at(i).name() == "remixsid"){
        s_cookies.remixsid = cookie->cookiesForUrl(authURL).at(i).value();
    }

    if (S.contains("access_token", Qt::CaseInsensitive)){
        authView->hide();

        const QNetworkCookieJar *cookie = authView->page()->networkAccessManager()->cookieJar();

        QUrl vkComUrl ("https://login.vk.com");
        for (int i=0; i < cookie->cookiesForUrl(vkComUrl).count(); i++){
            if (cookie->cookiesForUrl(vkComUrl).at(i).name() == "l")
                s_cookies.l = cookie->cookiesForUrl(vkComUrl).at(i).value();
            else
            if (cookie->cookiesForUrl(vkComUrl).at(i).name() == "p")
                s_cookies.p = cookie->cookiesForUrl(vkComUrl).at(i).value();
            else
            if (cookie->cookiesForUrl(vkComUrl).at(i).name() == "s")
                s_cookies.save = cookie->cookiesForUrl(vkComUrl).at(i).value().toInt();
        }

        QUrl temp = S.replace("#","?");
        s_sessionVars.access_token = temp.queryItemValue("access_token");
        s_sessionVars.uid = temp.queryItemValue("user_id");
        s_cookies.l = s_sessionVars.uid;
        Settings::instance()->setUid(s_cookies.l);
        Settings::instance()->setLastUid(s_cookies.l);
        Settings::instance()->saveProfile("net/id", s_cookies.l);
        Settings::instance()->saveProfile("net/psswd", s_cookies.p);
        Settings::instance()->saveProfile("net/sid", s_cookies.remixsid);

        temp.clear();
        S.clear();
        emit connected();

        return;
    }
    if (S.contains("access_denied", Qt::CaseInsensitive)) {
        authView->hide();
//        authView->setUrl(QUrl("about:blank"));
//        authView->load(QUrl(authURL));
//        authView->show();
        return;
    }

    if (s_cookies.remixsid == "none" || s_cookies.remixsid.isEmpty()){
        authView->show();
        return;
    }

    const QString& JS="javascript:document.getElementById('install_allow').onclick();";
    authView->page()->mainFrame()->evaluateJavaScript(JS);
}

void VK::slotConnected(){
    getUserInfo(s_sessionVars.access_token, s_sessionVars.uid);
    getAlbums(s_sessionVars.access_token, s_sessionVars.uid);
    getAudio(s_sessionVars.access_token, s_sessionVars.uid);
}

void VK::getUserInfo(QString access_token, QString uid){
    QStringList params;
    QString paramsStr;
    params.append("getProfiles.xml?");
    params.append("uids=" + uid);
    params.append("fields=photo_medium_rec");
    params.append("access_token=" + access_token);
    paramsStr = params.join("&");
    paramsStr = server + paramsStr;
    qDebug() << paramsStr;
    request.setUrl(paramsStr);
    http->get(request);
}

void VK::getAlbums(QString access_token, QString uid){
    QStringList params;
    QString paramsStr;
    params.append("audio.getAlbums.xml?");
    params.append("uid=" + uid);
    params.append("access_token=" + access_token);
    paramsStr = params.join("&");
    paramsStr = server + paramsStr;
    qDebug() << paramsStr;
    request.setUrl(paramsStr);
    http->get(request);
}

void VK::getAudio(QString access_token, QString uid){
    QStringList params;
    QString paramsStr;
    params.append("audio.get.xml?");
    params.append("uid="+uid);
    params.append("need_user=0");
    params.append("count=9999");                                             //126
    params.append("offset=0");
    params.append("access_token="+access_token);
    paramsStr = params.join("&");
    paramsStr = server + paramsStr;
    qDebug() << paramsStr;
    request.setUrl(paramsStr);
    http->get(request);
}

void VK::slotResponse(QNetworkReply *reply){
    if (reply->url().path() == "/method/getProfiles.xml") {
        userParse(reply->readAll());
    }

    if (reply->url().path() == "/method/audio.getAlbums.xml") {
        albumsParse(reply->readAll());
    }

    if (reply->url().path() == "/method/audio.get.xml") {
        audioParse(reply->readAll());
    }
}

void VK::userParse(QByteArray xml){
    QDomDocument domDoc;
    domDoc.setContent(xml);
    QDomElement domElem = domDoc.documentElement();
    QDomNodeList userXML = domElem.elementsByTagName ("user");
    QString picture;

    loginMVars s_loginMVars;

    for (int i = 0; i < userXML.count(); i++) {
            QDomElement userElem = userXML.at(i).toElement();
            s_loginMVars.id = userElem.elementsByTagName("uid").at(0).toElement().text();
            s_loginMVars.name = userElem.elementsByTagName("first_name").at(0).toElement().text() +
                    " " + userElem.elementsByTagName("last_name").at(0).toElement().text();
            picture = userElem.elementsByTagName("photo_medium_rec").at(0).toElement().text();

            if ((!s_loginMVars.id.isEmpty()) &&
                (!s_loginMVars.name.isEmpty()) &&
                (!picture.isEmpty())){

                QNetworkAccessManager ava;
                QNetworkReply * reply = ava.get( QNetworkRequest( picture ) );
                QEventLoop loop;
                connect( reply, SIGNAL(finished()), &loop, SLOT(quit()) );
                loop.exec();
                QString directoryAva = Settings::instance()->homeDirectory() + "profiles/" +s_loginMVars.id + "/ava.jpg";
                if ( reply->error() == QNetworkReply::NoError )
                {
                  QFile image( directoryAva );
                  image.open( QIODevice::WriteOnly );
                  image.write( reply->readAll() );
                  image.close();
                }

                s_loginMVars.pic = directoryAva;

                Settings::instance()->saveProfile("user/name", s_loginMVars.name);
                Settings::instance()->saveProfile("user/pic", s_loginMVars.pic);
                emit userDone(&s_loginMVars);
                return;
            }
    }
}

void VK::albumsParse(QByteArray xml){
    QDomDocument domDoc;
    domDoc.setContent(xml);
    QDomElement domElem = domDoc.documentElement();
    QDomNodeList albumXML = domElem.elementsByTagName ("album");

    albums s_albums;
    albumsHash.clear();

    for(int i=0; i < albumXML.count(); i++){
        QDomElement albumElem = albumXML.at(i).toElement();
        s_albums.owner_id = albumElem.elementsByTagName("owner_id").at(0).toElement().text();
        s_albums.id       = albumElem.elementsByTagName("album_id").at(0).toElement().text();
        s_albums.title    = albumElem.elementsByTagName("title").at(0).toElement().text();

        if (!s_albums.owner_id.isEmpty() &&
            !s_albums.id.isEmpty() &&
            !s_albums.title.isEmpty()){

            albumsHash.insert(s_albums.id, s_albums);
            s_albums.owner_id.clear();
            s_albums.id.clear();
            s_albums.title.clear();
        }
    }

    emit albumsDone(&albumsHash);
}

void VK::audioParse(QByteArray xml){
    QDomDocument domDoc;
    domDoc.setContent(xml);
    QDomElement domElem = domDoc.documentElement();
    QDomNodeList audioXML = domElem.elementsByTagName ("audio");

    audio s_audio;

    for(int i=0; i < audioXML.count(); i++){
            QDomElement audioElem = audioXML.at(i).toElement();
            s_audio.aid         = audioElem.elementsByTagName("aid").at(0).toElement().text();
            s_audio.owner_id    = audioElem.elementsByTagName("owner_id").at(0).toElement().text();
            s_audio.artist      = audioElem.elementsByTagName("artist").at(0).toElement().text();
            s_audio.title       = audioElem.elementsByTagName("title").at(0).toElement().text();
            s_audio.duration    = audioElem.elementsByTagName("duration").at(0).toElement().text().toInt();
            s_audio.url         = audioElem.elementsByTagName("url").at(0).toElement().text();
            s_audio.album_id    = audioElem.elementsByTagName("album").at(0).toElement().text();

            if ((!s_audio.aid.isEmpty()) &&
                (!s_audio.owner_id.isEmpty()) &&
                (!s_audio.artist.isEmpty()) &&
                (!s_audio.title.isEmpty()) &&
                (!s_audio.url.isEmpty())){

                if(s_audio.artist.contains("&#39;",Qt::CaseSensitive))
                    s_audio.artist.replace("&#39;","'",Qt::CaseSensitive);
                if(s_audio.artist.contains("&quot;",Qt::CaseSensitive))
                    s_audio.artist.replace("&quot;","\"",Qt::CaseSensitive);
                if(s_audio.artist.contains("&amp;",Qt::CaseSensitive))
                    s_audio.artist.replace("&amp;","&",Qt::CaseSensitive);
                if(s_audio.artist.contains("&#33;",Qt::CaseSensitive))
                    s_audio.artist.replace("&#33;","!",Qt::CaseSensitive);

                if(s_audio.title.contains("&#39;",Qt::CaseSensitive))
                    s_audio.title.replace("&#39;","'",Qt::CaseSensitive);
                if(s_audio.title.contains("&quot;",Qt::CaseSensitive))
                    s_audio.title.replace("&quot;","\"",Qt::CaseSensitive);
                if(s_audio.title.contains("&amp;",Qt::CaseSensitive))
                    s_audio.title.replace("&amp;","&",Qt::CaseSensitive);
                if(s_audio.title.contains("&#33;",Qt::CaseSensitive))
                    s_audio.title.replace("&#33;","!",Qt::CaseSensitive);

                if (!s_audio.album_id.isEmpty()) {
                    audioHash.insert(s_audio.album_id, s_audio);
                } else {
                    audioHash.insert(tr("0"), s_audio);
                }

                s_audio.aid.clear();
                s_audio.owner_id.clear();
                s_audio.artist.clear();
                s_audio.title.clear();
                s_audio.duration = 0;
                s_audio.url.clear();
                s_audio.album_id.clear();
            }
    }
    emit audioDone(&audioHash);
}

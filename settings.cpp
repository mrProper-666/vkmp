#include "settings.h"

Settings *Settings::aInstance = 0;

Settings *Settings::instance()
{
    if (!aInstance) aInstance = new Settings();
    return aInstance;
}

void Settings::destroy() {
   if (aInstance)
      delete aInstance, aInstance = 0;
}

Settings::Settings()
{
    //настройки пользователей
    home = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)+"/.vkMP/";

    qDebug() << "save settings to " << home;
    //домашний каталог программы
    if (QApplication::applicationDirPath() == "/usr/bin")
        homeApp = "/usr/share/vkmp/";
    else
        homeApp = QApplication::applicationDirPath() + "/";
}


void Settings::setUid(const QString &uid){
    this->id = uid;
}

void Settings::saveProfile(const QString &key, const QVariant &value){
    //настройки профиля
    QSettings settings(home + "/profiles/" + id + "/userSettings.ini", QSettings::IniFormat);
    settings.setValue(key, value);
}

void Settings::setLastUid(const QVariant &value){
    QSettings settings(home + "/settings.ini", QSettings::IniFormat);
    settings.setValue("id", value);
}

QVariant Settings::getLastUid(){
    QSettings settings(home + "settings.ini", QSettings::IniFormat);
    return settings.value("id", "0");
}

QVariant Settings::loadProfile(const QString &key, const QVariant &defaultValue){
    //настройки профиля
    QSettings settings(home + "profiles/" + id + "/userSettings.ini", QSettings::IniFormat);
    return settings.value(key, defaultValue);
}

QString Settings::homeAppDirectory(){
    return homeApp;
}

QString Settings::homeDirectory(){
    return home;
}

QStringList Settings::profilesIds(){
    QDir tempDir(home + "profiles/");

    return tempDir.entryList();
}

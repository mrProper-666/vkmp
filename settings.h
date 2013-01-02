#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDebug>

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings *instance();
    static void destroy();
    void setUid(const QString &uid);
    void setLastUid(const QVariant &value);
    void saveProfile(const QString &key, const QVariant &value);
    QVariant getLastUid();
    QVariant loadProfile(const QString &key, const QVariant &defaultValue);
    QString homeAppDirectory();
    QString homeDirectory();
    QStringList profilesIds();

private:
    Settings();
    static Settings *aInstance;
    QString home;
    QString homeApp;
    QString id;

signals:

public slots:

};

#endif // SETTINGS_H

#pragma once
#include "tkvsdatabasepool.h"
#include "turlroute.h"
#include <QByteArray>
#include <QEventLoop>
#include <QObject>
#include <QtTest/QtTest>
#include <TAppSettings>
#include <TSystemGlobal>
#include <TStdErrSystemLogger>
#include <TWebApplication>
#ifdef QT_SQL_LIB
#include "tsqldatabasepool.h"
#include <TActionThread>
#endif
#ifdef Q_OS_LINUX
#include <TMultiplexingServer>
#endif


#define TF_TEST_MAIN(TestObject) TF_TEST_SQL_MAIN(TestObject, true);

#if QT_VERSION < 0x060000
#include <QTextCodec>

#define TF_TEST_SQL_MAIN(TestObject, EnableTransactions)                                                               \
    int main(int argc, char *argv[])                                                                                   \
    {                                                                                                                  \
        class Thread : public TActionThread {                                                                          \
        public:                                                                                                        \
            Thread() : TActionThread(0), returnCode(0) { }                                                             \
            volatile int returnCode;                                                                                   \
                                                                                                                       \
        protected:                                                                                                     \
            virtual void run()                                                                                         \
            {                                                                                                          \
                setTransactionEnabled(EnableTransactions);                                                             \
                TestObject obj;                                                                                        \
                returnCode = QTest::qExec(&obj, QCoreApplication::arguments());                                        \
                commitTransactions();                                                                                  \
                for (QMap<int, TSqlTransaction>::iterator it = sqlDatabases.begin(); it != sqlDatabases.end(); ++it) { \
                    it.value().database().close(); /* close SQL database */                                            \
                }                                                                                                      \
                for (QMap<int, TKvsDatabase>::iterator it = kvsDatabases.begin(); it != kvsDatabases.end(); ++it) {    \
                    it.value().close(); /* close KVS database */                                                       \
                }                                                                                                      \
                QEventLoop eventLoop;                                                                                  \
                while (eventLoop.processEvents()) {                                                                    \
                }                                                                                                      \
            }                                                                                                          \
        };                                                                                                             \
        TWebApplication app(argc, argv);                                                                               \
        QByteArray codecName = Tf::appSettings()->value(Tf::InternalEncoding).toByteArray();                           \
        QTextCodec *codec = QTextCodec::codecForName(codecName);                                                       \
        QTextCodec::setCodecForLocale(codec);                                                                          \
        Tf::setupSystemLogger(new TStdErrSystemLogger);                                                                \
        app.setDatabaseEnvironment("test");                                                                            \
        TUrlRoute::instance();                                                                                         \
        TSqlDatabasePool::instance();                                                                                  \
        TKvsDatabasePool::instance();                                                                                  \
        INIT_MULTIPLEXING_SERVER();                                                                                    \
        Thread thread;                                                                                                 \
        thread.start();                                                                                                \
        thread.wait();                                                                                                 \
        _exit(thread.returnCode);                                                                                      \
        return thread.returnCode;                                                                                      \
    }

#else

#define TF_TEST_SQL_MAIN(TestObject, EnableTransactions)                                                               \
    int main(int argc, char *argv[])                                                                                   \
    {                                                                                                                  \
        class Thread : public TActionThread {                                                                          \
        public:                                                                                                        \
            Thread() : TActionThread(0), returnCode(0) { }                                                             \
            volatile int returnCode;                                                                                   \
                                                                                                                       \
        protected:                                                                                                     \
            virtual void run()                                                                                         \
            {                                                                                                          \
                setTransactionEnabled(EnableTransactions);                                                             \
                TestObject obj;                                                                                        \
                returnCode = QTest::qExec(&obj, QCoreApplication::arguments());                                        \
                commitTransactions();                                                                                  \
                for (QMap<int, TSqlTransaction>::iterator it = sqlDatabases.begin(); it != sqlDatabases.end(); ++it) { \
                    it.value().database().close(); /* close SQL database */                                            \
                }                                                                                                      \
                for (QMap<int, TKvsDatabase>::iterator it = kvsDatabases.begin(); it != kvsDatabases.end(); ++it) {    \
                    it.value().close(); /* close KVS database */                                                       \
                }                                                                                                      \
                QEventLoop eventLoop;                                                                                  \
                while (eventLoop.processEvents()) {                                                                    \
                }                                                                                                      \
            }                                                                                                          \
        };                                                                                                             \
        TWebApplication app(argc, argv);                                                                               \
        Tf::setupSystemLogger(new TStdErrSystemLogger);                                                                \
        app.setDatabaseEnvironment("test");                                                                            \
        TUrlRoute::instance();                                                                                         \
        TSqlDatabasePool::instance();                                                                                  \
        TKvsDatabasePool::instance();                                                                                  \
        INIT_MULTIPLEXING_SERVER();                                                                                    \
        Thread thread;                                                                                                 \
        thread.start();                                                                                                \
        thread.wait();                                                                                                 \
        _exit(thread.returnCode);                                                                                      \
        return thread.returnCode;                                                                                      \
    }
#endif


#if QT_VERSION < 0x060000

#define TF_TEST_SQLLESS_MAIN(TestObject)                                                              \
    int main(int argc, char *argv[])                                                                  \
    {                                                                                                 \
        TWebApplication app(argc, argv);                                                              \
        Tf::setupSystemLogger(new TStdErrSystemLogger);                                               \
        QByteArray codecName = Tf::appSettings()->value(Tf::InternalEncoding).toByteArray();          \
        QTextCodec *codec = QTextCodec::codecForName(codecName);                                      \
        QTextCodec::setCodecForLocale(codec);                                                         \
        TestObject tc;                                                                                \
        return QTest::qExec(&tc, argc, argv);                                                         \
    }

#else

#define TF_TEST_SQLLESS_MAIN(TestObject)                                                              \
    int main(int argc, char *argv[])                                                                  \
    {                                                                                                 \
        TWebApplication app(argc, argv);                                                              \
        Tf::setupSystemLogger(new TStdErrSystemLogger);                                               \
        TestObject tc;                                                                                \
        return QTest::qExec(&tc, argc, argv);                                                         \
    }

#endif


#ifdef Q_OS_LINUX
#define INIT_MULTIPLEXING_SERVER() TMultiplexingServer::instantiate(0)
#else
#define INIT_MULTIPLEXING_SERVER()
#endif

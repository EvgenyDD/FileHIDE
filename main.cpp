#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <QCoreApplication>
#include <QDir>
#include <QThread>
#include <QElapsedTimer>
#include <QDateTime>
#include <stdint.h>


QByteArray workLog;

#define WAITANDEXIT()   {   qDebug() << "--------------------------------------------------";\
                            qDebug() << "             Press <ENTER> to exit";\
                            getchar();\
                            return 0;}


#define createDefaultNestedLevelsettingsFileFile() SL[0] ="3\n";\
                                        settingsFile.resize(0);\
                                        QTextStream outStream1(&settingsFile);\
                                        outStream1 << SL.join("");\
                                        settingsFile.close();

#define createDefaultSettingsFile()     QString ss = "nested levels=2";\
                                        QStringList outStringList2; \
                                        outStringList2.append("3\n");\
                                        outStringList2.append("Dear friend!\n");\
                                        outStringList2.append("I want to play a game\n");\
                                        outStringList2.append("Only one file in these folders is correct\n");\
                                        outStringList2.append("Enjoy!!!");\
                                        settingsFile.resize(0);\
                                        QTextStream outStream2(&settingsFile);\
                                        outStream2 << outStringList2.join("");\
                                        settingsFile.close();

#define removeSysSymbolsInPath(X)   X.remove(':');\
                                    X.remove('\\');\
                                    X.remove('/');\
                                    X.remove('*');\
                                    X.remove('?');\
                                    X.remove('"');\
                                    X.remove('<');\
                                    X.remove('>');\
                                    X.remove('|');\
                                    X.remove(0x0D);\
                                    X.remove(0x0A);

QByteArray warpFileArray(QByteArray *Array)
{
    QByteArray out = *Array;
    uint32_t size = out.size();

    for(uint32_t i=0; i<size/2; i++)
    {
        uint32_t offset = i*2;
        int value = out.at(offset);
        char newWarpValue = 0;
        while(value == newWarpValue)
            newWarpValue = rand()%256;
        out[offset] = newWarpValue;
    }
    return out;
}


int WriteFileToSubFolder(QString pathCommon,
                          QString subFolder1,
                          int iterator,
                          QString subFolder2,
                          QString fileName,
                          QByteArray *file,
                          bool isOriginalFile)
{
    removeSysSymbolsInPath(subFolder1);
    removeSysSymbolsInPath(subFolder2);
    if(subFolder1.size() == 0)
        return 1;

    for(int i = subFolder1.size() - 1; i != 0; i--)
    {
        if(subFolder1.at(i) == ' ')
            subFolder1.remove(i, 1);
        else
            break;
    }
    for(int i = 0; i < subFolder1.size(); i++)
    {
        if(subFolder1.at(i) == ' ')
            subFolder1.remove(i, 1);
        else
            break;
    }

    if(subFolder1 == "")
        return 1;

    QString pathSubFolder = pathCommon;
    //pathSubFolder.append("\\");
    pathSubFolder.append(QString::number(iterator).rightJustified(2, '0'));
    pathSubFolder.append("___");
    pathSubFolder.append(subFolder1);
    pathSubFolder.append("\\");
    pathSubFolder.append(subFolder2);
    if(subFolder2 != "")
        pathSubFolder.append("\\");

    //create subfolder if not exist
    QDir dir(pathSubFolder);

    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QString outFilePath = pathSubFolder;
    outFilePath.append(fileName);

    QFile outFile(outFilePath);
    if(!outFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't create output file " << outFilePath;
        return 1;
    }

    outFile.write(*file);
    if(isOriginalFile)
    {
        workLog.append("\r\nRight file is situated in folder: ");
        workLog.append(outFilePath);
    }

    outFile.close();
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QElapsedTimer timerSeed;
    srand(timerSeed.elapsed());

    if(argc == 2)
    {
        QString fileAddress = QCoreApplication::arguments().at(1);

        QFile file(fileAddress);
        if(!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "Can't open file. Permissions error";
            WAITANDEXIT();
        }

        workLog.append("\r\nGlobal file address: ");
        workLog.append(fileAddress);
qDebug() << "Global file address: " << fileAddress;

        QString pathFolder = fileAddress;
        QString inversedName;
        QString fileName;
        for(int i = pathFolder.length()-1; i != 0; i--)
        {
            if((pathFolder.at(i) == '\\') || (pathFolder.at(i) == '/'))
            {
                for(int i=0; i<inversedName.size(); i++)
                    fileName.append(inversedName.at(inversedName.size() - i - 1));
                break;
            }
            else
            {
                inversedName.append(fileAddress.at(i));
                pathFolder.remove(i, 1);
            }
        }

        workLog.append("\r\nPath to file folder: ");
        workLog.append(pathFolder);
qDebug() << "Path to file folder: " << pathFolder;
        workLog.append("\r\nFile name: ");
        workLog.append(fileName);
qDebug() << "File name: " << fileName;

        QByteArray origFileArray = file.readAll();
        
        QFile settingsFile("settings.ini");
        if(!settingsFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "Can't open settings.ini file.\n I will create default settings file.\n You can correct if by yourself.";
            settingsFile.open(QIODevice::WriteOnly);
            createDefaultSettingsFile();
            settingsFile.close();
            WAITANDEXIT();
        }

        QStringList settingsFileStrings;
        while(!settingsFile.atEnd())
            settingsFileStrings.append(settingsFile.readLine());

        if(settingsFile.size() < 2)
        {
            qDebug() << "Too small settings.ini size.\nI will create default settings file.\n You can correct if by yourself.";
            createDefaultSettingsFile();
            settingsFile.close();
            WAITANDEXIT();
        }

        workLog.append("\r\nSettings number of lines ");
        workLog.append(QString::number(settingsFileStrings.size()));
qDebug() << "Settings number of lines " << settingsFileStrings.size();

        int numOfSubfolders = settingsFileStrings.size() - 1;
        int nestingNumber = settingsFileStrings.at(0).toInt();
        workLog.append("\r\nNesting number = ");
        workLog.append(QString::number(nestingNumber));
        workLog.append("\r\nNumber of folders to create: ");
        workLog.append(QString::number(numOfSubfolders * nestingNumber));
qDebug() << "Nesting number = " << nestingNumber;
qDebug() << "Number of folders to create: " << numOfSubfolders * nestingNumber;

        if(nestingNumber < 1 || nestingNumber > 10)
        {
            qDebug() << "Wrong nesting number in settings.ini, it should be in range from 1 to 10";
            WAITANDEXIT();
        }
        if(numOfSubfolders < 2 || numOfSubfolders > 100)
        {
            qDebug() << "Wrong number of subfolders in settings.ini, it should be in range from 2 to 100";
            WAITANDEXIT();
        }

        int cntRightFile = (numOfSubfolders * nestingNumber * 1 / 3) + rand() % (numOfSubfolders * nestingNumber * 2 / 3);
        if(cntRightFile >= (numOfSubfolders * nestingNumber))
            cntRightFile = numOfSubfolders * nestingNumber - 1;

        for(int i = 0, totCnt = 0; i < numOfSubfolders; i++)
        {
            QByteArray fakeFileArray;

            if(nestingNumber == 1)
            {
                fakeFileArray = warpFileArray(&origFileArray);
                WriteFileToSubFolder(pathFolder, settingsFileStrings.at(1 + i), i, "", fileName,
                                     (totCnt == cntRightFile) ? &origFileArray : &fakeFileArray,
                                     (totCnt == cntRightFile) ? true : false);
                totCnt++;
            }
            else
            {
                for(int j = 0; j < nestingNumber; j++)
                {
                    QString tempName;
                    for(int k=0; k<18; k++)
                    {
                        switch(rand()%3)
                        {
                        case 0:
                            tempName.append(rand()%10 + '0');
                            break;
                        case 1:
                            tempName.append(rand()%27 + 'a');
                            break;
                        case 2:
                            tempName.append(rand()%27 + 'A');
                            break;
                        }
                    }

                    fakeFileArray = warpFileArray(&origFileArray);
                    WriteFileToSubFolder(pathFolder, settingsFileStrings.at(1 + i), i, tempName, fileName,
                                         (totCnt == cntRightFile) ? &origFileArray : &fakeFileArray,
                                         (totCnt == cntRightFile) ? true : false);
                    totCnt++;
                }
            }
        }

        QString pathToLogFolder = QCoreApplication::arguments().at(1);

        for(int i = pathToLogFolder.length()-1; i != 0; i--)
        {
            if((pathToLogFolder.at(i) == '\\') || (pathToLogFolder.at(i) == '/'))
                break;
            else
                pathToLogFolder.remove(i, 1);
        }

        pathToLogFolder.append("log ");
        pathToLogFolder.append(fileName);
        pathToLogFolder.append(" ");
        QDateTime time = QDateTime::currentDateTime();

        QString tempTD = time.date().toString("dd.MM.yyyy");
        removeSysSymbolsInPath(tempTD);
        pathToLogFolder.append(tempTD);
        pathToLogFolder.append(" ");
        tempTD = time.time().toString();
        removeSysSymbolsInPath(tempTD);
        pathToLogFolder.append(tempTD);
        pathToLogFolder.append(".log");
        QFile logFile(pathToLogFolder);

        if(logFile.open(QIODevice::WriteOnly))
        {
            logFile.write(workLog);
            logFile.close();
        }
    }
    else
    {
        qDebug() << "================ Wrong usage!!! ==================";
        qDebug() << "You should pull the target file to this .exe file!";
        WAITANDEXIT();
    }

    qDebug() << "Exiting......";
    return 0;
    return a.exec();
}

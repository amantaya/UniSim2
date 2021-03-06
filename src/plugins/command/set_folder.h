#ifndef SET_FOLDER_H
#define SET_FOLDER_H
#include <base/command.h>

namespace command {

class set_folder : public base::Command
{
public:
    set_folder(QString name, QObject *parent);
    static void showAllFolders();
private:
    void doExecute();
    void showFolder(QString folderType);
    void setFolder(QString folderType, QString folderPath);
};

}

#endif

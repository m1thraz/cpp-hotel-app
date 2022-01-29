#ifndef DATABASE_H
#define DATABASE_H
#include <string>

class Database
{
public:
    Database();
    void connectToDatabase();
    void closeDatabaseConnection();
    bool loginQuery(const int id, const std::string passwort);

    void createDatabaseTables();
    void createDatabaseEntries();

    int getBestandID(int zimmernummer);
};

#endif // DATABASE_H

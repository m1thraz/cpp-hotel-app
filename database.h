#ifndef DATABASE_H
#define DATABASE_H
#include <string>

class Database
{
public:
    Database();
    void connectToDatabase();
    void closeDatabaseConnection();
    void loginQuery(const int id, const std::string passwort);

    void createDatabaseTables();
    void createDatabaseEntries();
};

#endif // DATABASE_H

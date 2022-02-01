#ifndef VERIFIER_H
#define VERIFIER_H


class verifier
{
public:
    verifier();
    bool verifyKundenIDExists(int kundenID);
    bool verifyMitarbeiterIDExists(int mitarbeiterID);
    bool verifyBestandIDExists(int bestandID);
    bool verifyZimmernummerExists(int zimmernummer);
    bool verifyZimmernummerDoesntExist(int zimmernummer);
    bool verifyZimmerIDExists(int zimmerID);
    bool verifyBuchungExists(int buchungsID, int kundenID);
};

#endif // VERIFIER_H

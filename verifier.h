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
    bool verifyKundeIsCheckedIn(int buchungsID);
    bool verifyKundeIsCheckedOut(int buchungsID);
    bool verifyBuchungsIDExists(int buchungsID);
    bool verifyRechnungExists(int buchungsID);
};

#endif // VERIFIER_H

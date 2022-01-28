#ifndef VERIFIER_H
#define VERIFIER_H


class verifier
{
public:
    verifier();
    bool verifyKundenIDExists(int kundenID);
    bool verifyMitarbeiterIDExists(int mitarbeiterID);
    bool verifyBestandIDExists(int bestandID);
};

#endif // VERIFIER_H

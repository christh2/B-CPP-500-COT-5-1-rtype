#include "../../include/network/server/binary_protocol.hpp"

std::string BinaryProtocol::crypt_message(Message message)
{
    /*Déclarer un ostringstream pour stcocker message dans _crypt*/
    std::ostringstream _crypt;
    /*Préparer _crypt à reçevoir le msg crypté avec la archive*/
    boost::archive::binary_oarchive archive(_crypt);
    /*Prendre le msg et le crypté avec archive. Archive va stocker le msg crypté dans _crypt*/
    archive << message;
    /*Récupérer le message crypté en string dans out_crypt*/
    std::string out_crypt = _crypt.str();
    /*Retourner le msg crypté en forme de string*/
    return out_crypt;
}

Message BinaryProtocol::decrypt_message(std::string message_crypted)
{
    try
    {
         /*Déclarer Ma structre*/
        Message take_message;
        /*Prendre le msg string crypté dasns _decrypt*/
        std::istringstream _decrypt(message_crypted);
        /*Prendre le message crypté et décrypter avec _decrypt*/
        boost::archive::binary_iarchive iarchive(_decrypt);
        /*Décrypté le msg et mettre remplir ma structure*/
        iarchive >> take_message;
        return take_message;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return Message{};
    }
    
}
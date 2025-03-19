#pragma once
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <sstream>
#include <array>
#include <iostream>
#include <cstdint>

/*  header
    0x000 => Error message
    0x01 => Connection
    0x02 => Login
    0x03 => Position and Id
    0x04 => Start
    0x04A => Client ok
    0x05 => Moves {
                    0x05A => Up
                    0x05B => Down
                    0x05C => Left
                    0x05D => Right
                  }
    0x051 => Send position of one client
    0x06 => Shoot
    0x061A => Shoot bonus 1
    0x062A => Shoot bonus 2
    0x063A => Shoot bonus 3
    0x06B => Long Shoot
    0x061B => Long Shoot 1
    0x062B => Long Shoot 2
    0x063B => Long Shoot 3
    0x06C => Laser shoot
    0x07 => Someone is killed
    0x07A => Player Killed
    0x07B => Enemi Killed
    0x09 => Ennemi {
                        0x09A  => Battle
                        0x09B  => Papillon
                        0x09C  => Commandant 
                        0x091A => Alien
                        0x091B => Pique
                        0x091C => Robot
                        0x091F  => First Boss
                        0x092F => Second Boss
                        0x093F => Third Boss
                        0x094F => Fourth Boss
                        0x091D => minion
                    }
    0x099 => Ennemi shoot
    0x010 => Deconnexion Player
    0x011 => Server shutdown
    0x015 => Game Over
    0x016 => Restart
    0x019A => Bonus
    0x019B => Bonus taken
    0x020 => Next level
     0x08 => Request AI to play {
                    0x08A => Enable AI for the player
                    0x08B => Disable AI for the player
                  }
    0x019C => Bonus shield (temporarily protects the player from damage)
    0x019D => Bonus speed boost (temporarily increases the player's movement speed)
    etc...
*/

/*The main structure through which data is sent....*/

struct Message
{
    std::uint16_t header;
    std::uint16_t id;
    int pos_x;
    int pos_y;

    template<class Archive>
    void serialize(Archive &arch, const unsigned int version) {
        arch &header;
        arch &id;
        //arch &message;
        arch &pos_x;
        arch &pos_y;
    }
};


class BinaryProtocol
{
public:
    BinaryProtocol () {};
    ~BinaryProtocol () {};
    std::string crypt_message(Message message);
    Message decrypt_message(std::string message_crypted);
};

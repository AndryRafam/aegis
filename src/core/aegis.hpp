#ifndef AEGIS_HPP
#define AEGIS_HPP

#include <string>
#include <random>
#include <string_view>
#include <cryptopp/osrng.h>

class Aegis {
    public:
        Aegis() = default;
        virtual ~Aegis() = default;

        enum class AppMode {
	        Exit,
	        Proceed,
	        Go_Back
        };
        AppMode action_selection{AppMode::Proceed}; // default initialization
        
        void clearScreen();
        char getch();
        
    protected:
        bool aes_cipher(const std::string& mode, const std::string& filePath, const std::string& password);
        bool sm4_cipher(const std::string& mode, const std::string& filePath, const std::string& password);
        bool xchacha20_cipher(const std::string& mode, const std::string& filePath, const std::string& password);
        
        static CryptoPP::AutoSeededRandomPool& get_rng();
        int getRandomInt(int min, int max);
        std::string generatePassword(int length);
        void setEcho(bool enable);
        bool askYN(std::string_view prompt);
        void secure_clear(std::string& s); // wipe string contents
        std::string getValidPath();
        void about();
};

#endif

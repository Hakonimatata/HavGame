#include "sound&music.h"

void playSound(const char* filename) {
    // Funksjon for å spille av lyden i en egen tråd
    auto soundThreadFunc = [filename]() {
        // Initialiserer SDL med SDL_INIT_AUDIO flagget
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;

        if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) {
            std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
            SDL_Quit(); // Vær sikker på å rydde opp SDL når du ikke lenger trenger det
            return;
        }

        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
        if (deviceId == 0) {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
            SDL_FreeWAV(wavBuffer);
            SDL_Quit(); // Rydde opp SDL
            return;
        }

        int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        SDL_PauseAudioDevice(deviceId, 0);

        // Vent til lyden er ferdig å spille
        while (SDL_GetQueuedAudioSize(deviceId) > 0) {
            SDL_Delay(100); // Vent 100 ms før du sjekker igjen
        }

        // Lukk lydenheten
        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
        // SDL_Quit(); // Rydde opp SDL, fjerner fordi den lukker vinduet
    };

    // Start en ny tråd for å spille av lyden
    std::thread soundThread(soundThreadFunc);

    // Ikke vent på lydtråden før funksjonen returnerer
    soundThread.detach();
}



void playSoundLoop(const char* filename) {
    // Funksjon for å spille av lyden i en egen tråd
    auto soundThreadFunc = [filename]() {
        // Initialiserer SDL med SDL_INIT_AUDIO flagget
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;

        if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) {
            std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
            SDL_Quit(); // Vær sikker på å rydde opp SDL når du ikke lenger trenger det
            return;
        }

        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
        if (deviceId == 0) {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
            SDL_FreeWAV(wavBuffer);
            SDL_Quit(); // Rydde opp SDL
            return;
        }

        // Spill lyden i en loop
        int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        SDL_PauseAudioDevice(deviceId, 0);

        while (true) {
            // Vent til lyden er ferdig å spille
            while (SDL_GetQueuedAudioSize(deviceId) > 0) {
                SDL_Delay(100); // Vent 100 ms før du sjekker igjen
            }

            // Snu lydfilen tilbake til starten for å spille av igjen
            SDL_ClearQueuedAudio(deviceId);
            SDL_QueueAudio(deviceId, wavBuffer, wavLength);
            SDL_PauseAudioDevice(deviceId, 0);
        }

        // Aldri nådd, men her for å vise hvordan man rydder opp
        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
        SDL_Quit(); // Rydde opp SDL
    };

    // Start en ny tråd for å spille av lyden
    std::thread soundThread(soundThreadFunc);

    // Ikke vent på lydtråden før funksjonen returnerer
    soundThread.detach();
}
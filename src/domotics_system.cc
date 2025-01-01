
#include "../include/domotics_system.h"
#include <fstream>
#include <sstream>
#include <string> // Aggiungi questo include
#include <vector>

/*
*   std::vector<T>   DomoticsSystem::getDevices () {
      std::ifstream file(percorsoFile);
    if (!file) {
        throw std::runtime_error("Impossibile aprire il file: " + percorsoFile);
    }

    std::vector<Dispositivo> dispositivi;
    std::string line, nome, durataAccensione, produzioneConsumo;

    // Parsing manuale del JSON (semplice)
    while (std::getline(file, line)) {
        if (line.find("\"Nome\":") != std::string::npos) {
            nome = line.substr(line.find(":") + 2);
            nome = nome.substr(1, nome.size() - 3); // Rimuove virgolette e
virgola finale } else if (line.find("\"DurataAccensione\":") !=
std::string::npos) { durataAccensione = line.substr(line.find(":") + 2);
            durataAccensione = durataAccensione.substr(1,
durataAccensione.size() - 3); } else if (line.find("\"ProduzioneConsumo\":") !=
std::string::npos) { produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1,
produzioneConsumo.size() - 3);

            // Crea il dispositivo in base alla durataAccensione
            if (durataAccensione == "Manuale") {
                dispositivi.emplace_back(ManualDevice(nome, durataAccensione,
produzioneConsumo)); } else { dispositivi.emplace_back(CPDevice(nome,
durataAccensione, produzioneConsumo));
            }
        }
    }

    return dispositivi;
    }

*/

// DomoticsSystem::DomoticsSystem() {
//     devices = "";
// }

// void DomoticsSystem::setDevices() {
//   std::ifstream file("./assets/devices.json");
//   if (!file) {
//     throw std::runtime_error("il file devices.json non esiste");
//   }
//   std::string line, nome, durataAccensione, produzioneConsumo;

//   // manual parsing of the JSON (simple)
//   while (std::getline(file, line)) {
//     if (line.find("\"Nome\":") != std::string::npos) {
//       nome = line.substr(line.find(":") + 2);
//       nome = nome.substr(1, nome.size() -
//                                 3); // Remove the quotes and the final comma
//     } else if (line.find("\"DurataAccensione\":") != std::string::npos) {
//       durataAccensione = line.substr(line.find(":") + 2);
//       durataAccensione =
//           durataAccensione.substr(1, durataAccensione.size() - 3);
//     } else if (line.find("\"ProduzioneConsumo\":") != std::string::npos) {
//       produzioneConsumo = line.substr(line.find(":") + 2);
//       produzioneConsumo =
//           produzioneConsumo.substr(1, produzioneConsumo.size() - 3);

//       // Create the device based on the durataAccensione
//       if (durataAccensione == "Manuale") {
//         devices += line;
//       } else {
//         devices += line;
//       }
//     }
//   }
// }

std::vector<Device *> DomoticsSystem::getDevices() const { return devices; }

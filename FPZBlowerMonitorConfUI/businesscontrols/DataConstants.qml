pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property string startInputIt:
    "Avvia\n
Parti\n
Lancia il sistema\n
Avvia il dispositivo\n
Metti in moto il sistema\n
Fai partire la soffiante\n
Accendi il dispositivo\n
Metti in funzione il sistema\n
Avvia il sistema\n
Fai partire il sistema\n
Accenditi\n
Avvia la soffiante\n
Attiva il dispositivo\n
Metti in moto la soffiante\n
Avvia la soffiante\n
Fai partire il dispositivo\n
Metti in funzione la soffiante\n
Avvia il dispositivo\n
Metti in moto il dispositivo\n
Fai partire la soffiante\n
Metti in funzione il dispositivo\n
Avvia la ventola\n
Fai partire la ventola\n
Accendi il sistema\n
Accendi"

    readonly property string stopInputIt:
    "Spegni soffiante\n
Spegni\n
Ferma\n
Stop\n
Fermati\n
Ferma soffiante\n
Ferma la soffiante\n
Spegni la soffiante\n
Stoppa\n
Stoppati\n
Stop soffiante\n
Spegni il dispositivo\n
Disattiva il sistema\n
Ferma il dispositivo\n
Arresta la soffiante\n
Interrompi il sistema\n
Spegniti\n
Spegni il ventilatore\n
Interrompi la soffiante\n
Ferma la ventola\n
Disattiva il dispositivo\n
Spegni la ventola\n
Disattiva la soffiante\n
Arresta la ventola\n
Smetti di ventilare\n
Disattiva la ventola\n
Smetti di soffiare"

    readonly property string increaseInputIt:
    "Aumenta la frequenza di {freq}\n
Aumenta la frequenza di {freq} hertz\n
Aumenta la velocita di {freq} hertz\n
Piu {freq}\n
Aumenta di {freq}\n
Alza la rotazione di {freq}\n
Incrementa di {freq}"

    readonly property string decreaseInputIt:
    "Diminuisci la frequenza di {freq}\n
Meno {freq}\n
Abbassa la velocita di {freq}\n
Abbassa la velocita di {freq} hertz\n
Abbassa la frequenza di {freq}\n
Abbassa la frequenza di {freq} hertz\n
Abbassa di {freq}\n
Riduci di {freq}"
    readonly property string setInputIt:
    "Imposta la frequenza a {freq}\n
Imposta la frequenza a {freq} hertz\n
Metti la frequenza a {freq}\n
Metti la velocita a {freq}\n
Metti la velocita a {freq} hertz\n
Imposta {freq}\n
Imposta {freq} hertz\n
Vai a {freq}\n
Vai a {freq} hertz\n
Passa a {freq}\n
Imposta la rotazione a {freq}\n
Imposta a {freq}"

    readonly property string startInputEn:
    "Start blower\n
Start\n
Turn on\n
Launch the system\n
Activate the device\n
Start the device\n
Turn on\n
Set the system in motion\n
Start the blower\n
Turn on the device\n
Put the system into operation\n
Start the system\n
Part\n
Light up\n
Put the device into operation\n
Turn on the system"

    readonly property string stopInputEn:
    "Stop blower\n
Stop\n
Blower stop\n
Stop the blower\n
Turn off\n
Turn off the blower\n
Switch off blower\n
Turn off the device\n
Deactivate the system\n
Stop the device\n
Stop the system\n
Turn off the fan\n
Stop the fan\n
Deactivate the device\n
Stop ventilating\n
Stop blowing"
    readonly property string increaseInputEn:
    "Increase frequency by {freq}\n
Increase the frequency by {freq}\n
Increase by {freq}\n
Add {freq}\n
Plus {freq}\n
Raise the rotation by {freq}\n
Increase the speed by {freq}\n
Increase the speed by {freq} hertz"
    readonly property string decreaseInputEn:
    "Reduce frequency by {freq}\n
Reduce {freq}\n
Reduce by {freq}\n
Less {freq}\n
Decrease the frequency by {freq}\n
Minus {freq}\n
Lower the speed by {freq}\n
Decrease the rotation by {freq}"
    readonly property string setInputEn:
    "Set frequency to {freq}\n
Set the frequency to {freq}\n
Set the frequency to {freq} hertz\n
Set {freq}\n
Frequency {freq}\n
Go to {freq}\n
Go to {freq} hertz\n
Put the speed at {freq}\n
Put the speed at {freq} hertz\n
Put the frequency to {freq}\n
Set the rotation to {freq}\n
Pass to {freq}"
}

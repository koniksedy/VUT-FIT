## Which parameters for DQN could be used:
- Estimation of probability of holding an area until next turn
    6 prvních výskytů možných útoků
        Každý bude mít tyto parametry:
            Šance na zabrání území
            Pravděpodobnost udržení území
            Počet okolních (nepřátelských) území
            Hodnota kostky na daném poli
            Hodnota soupeřovi kostky
    ...
    Hodnota sumy všech kostek

    Output:
        Útok na soupeře 1
        Útok na soupeře 2
        Útok na soupeře 3
        Útok na soupeře 4
        Útok na soupeře 5
        Útok na soupeře 6

    Reward:
        Více polí v mém držení než v předchozím kole +10
        Méně polí v mém držení než v předchozím kole -10
        Stejný počet polí jako v předchozím 0





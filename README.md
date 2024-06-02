# Polen-Detection

## Prezentare Generala

Programul este conceput pentru a clasifica imaginile in doua categorii: polen si fara polen, folosind proprietati de culoare HSV si RGB.

## Configurare Experimentala

### Pregatirea Datelor

Programul asteapta un set de date organizat in doua subfoldere intr-un director numit `dataset`. Subfolderele ar trebui sa fie denumite `pollen` si `nopollen`, continand imagini in format `.jpg`.

## Detalii Experimentale

### Metodologie

1. **Incarcarea Datelor**: Imaginile sunt incarcate din calea specificata a setului de date.
2. **Etichetare**: Imaginile sunt etichetate pe baza numelor folderelor (`pollen` sau `nopollen`).
3. **Calcularea Caracteristicilor**: Valorile medii HSV si RGB sunt calculate pentru fiecare imagine.
4. **Antrenament**: Valorile medii HSV si RGB sunt mediate pentru setul de antrenament pentru a crea praguri de clasa.
5. **Testare**: Imaginile din setul de testare sunt clasificate pe baza celor mai apropiate valori medii HSV si RGB.

### Functii

- **Matricea de Confuzie**: Matricea confuziei este afisata, aratand rezultatele clasificarii pentru fiecare clasa.
- **Calcularea Acuratetii**: Acuratetea este calculata si afisata pe baza setului de testare.

### Observatii

- **Calcularea Pragurilor**: Valorile pragurilor pentru HSV si RGB sunt calculate si stocate.
- **Clasificarea Imaginilor**: Imaginile sunt clasificate pe baza pragurilor calculate.
- **Acuratetea**: Acuratetea clasificarii este calculata si afisata.

## Concluzie

Programul clasifica eficient imaginile in categorii de polen si fara polen, pe baza proprietatilor HSV si RGB. Imbunatatiri suplimentare pot fi realizate prin incorporarea unor caracteristici aditionale sau optimizarea algoritmului de clasificare.

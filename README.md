# SO-Projekt3-PK
The third projekt from subject "Systemy Operacyjne" from Information Technologies studies 

PROJEKT NR 3 – SEMAFORY, KOMUNIKACJA TYPU PAMIĘĆ DZIELONA
Projekt składa się z dwóch programów uruchamianych niezależnie: producenta i konsumenta.
Producent zajmuje się produkcją towaru (np. liczb, znaków) i umieszczaniu ich we wspólnym
buforze (pamięć dzielona), który może pomieścić tylko jedną jednostkę towaru naraz.
Konsument pobiera towar (nie niszcząc bufora) i konsumuje go.

Aby panowała harmonia, muszą być spełnione dwa warunki:
- każda wyprodukowana jednostka towaru musi zostać skonsumowana,
- żadna jednostka towaru nie może być skonsumowana dwa razy (nawet jeśli konsument jest
szybszy niż producent).

Wykorzystując mechanizm semaforów zaimplementuj powyższe zadanie. Dla
zademonstrowania, że nie doszło do utraty lub zwielokrotnienia towaru niech producent
pobiera „surowiec” (liczby, znaki) do wytwarzania towaru z pliku tekstowego, a konsument
umieszcza pobrany towar w innym pliku tekstowym. Po zakończeniu działania programów
(wyczerpaniu zasobów „surowca”) oba pliki tekstowe powinny być identyczne. Do symulacji
różnych prędkości działania programów użyć np. funkcji sleep() z losową liczbą sekund.
Zaimplementuj obsługę przerwania wywołania funkcji systemowej semop().

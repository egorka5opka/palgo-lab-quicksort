Код сортировки в [sort.cpp](src/sort.cpp). Использовал parlaylib

Можно позапускать
```
./run.sh
```

Для этого нужен parlib локально, если репозиторий был склонирован без `--recurse-submodules`, то надо выполнить
```
git submodule update --init --recursive
```


# Результат тестов
(Большая часть кода тестов очевидно нейрослопная, но вроде норм)

На удивление, получилось, что просто `parlay::par_do` при вызове рекурсии уже дало ускорение в $3.311$, по сравнению с такой же однопоточной реализацией, по сравнению с `std::sort` в  $1.925$. Хотя эти числа довольно сильно скачут, в следующем запуске получилось вот так:
```
size=100000000, repetitions=5, workers=4
sequential mean: 16040.759 ms
parallel mean:   5377.123 ms
std::sort mean:  12400.132 ms
parallel speedup vs sequential: 2.983x
parallel speedup vs std::sort:  2.306x
```

На лекции обсуждалось, что в таких параллельных алгоритмах примерно всегда имеется смысл маленькие диапазоны обрабатывать однопоточно. Решил добавить это, сказав что маленькие это длины $128$. После этого стало получше, но кажется мой ноутбук слишком непостоянный для нормальных бенчмарков тут.

```
size=100000000, repetitions=5, workers=
sequential mean: 16100.668 ms
parallel mean:   4778.876 ms
std::sort mean:  12394.411 ms
parallel speedup vs sequential: 3.369x
parallel speedup vs std::sort:  2.594x
```

upd: оставил на ночь побольше повторений
```
size=100000000, repetitions=100, workers=4
sequential mean: 15922.621 ms
parallel mean:   5150.643 ms
std::sort mean:  12629.797 ms
parallel speedup vs sequential: 3.091x
parallel speedup vs std::sort:  2.452x
```
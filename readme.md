# Урок 1, Многопоточность
## Задание 1, Атомарная очередь клиентов
*Homework_1*

Необходимо модифицировать задание №1 к первому уроку таким образом, чтобы счетчик клиентов был атомарным.
Все операции со счетчиков должны быть атомарными.
Проверить работу с различными способами упорядочения доступа к памяти.

## Задание 2, Прогресс бар
*Homework_2*

Создать консольное приложение для имитации многопоточного расчёта.
Количество потоков, длина расчёта должны быть заданы переменными.
В консоль во время работы программы должно построчно для каждого потока выводиться:

- Номер потока по порядку;
- Идентификатор потока;
- Заполняющийся индикатор (наподобие прогресс-бара) визуализирующий процесс «расчёта»;
- По завершении работы каждого потока в соответствующей строке выводить суммарное время, затраченное при работке потока.

## Задание 3, Защищенный обмен данными
*Homework_3*

Создать класс Data, содержащий в качестве полей скалярные данные и мьютекс. 
Создать функцию swap, которая принимает ссылки на два объекта класса Data и обменивает их местами.
В функция необходимо сначала захватить мьютексы обоих объектов, а затем выполнеть обмен данными. 
Реализовать три варианта данной функции: при помощи lock, scoped_lock и unique_lock.

CPP-07
23.07.2023
[@J1n4ed](https://github.com/J1n4ed)
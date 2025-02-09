import random
import time

file = open("test.file", "w")

output = ""

names = ["Кривой", 
        "Косой",
        "Бравый",
        "Быстро",
        "Альфа",
        "Алый",
        "Ящик",
        "Ярый",
        "1Косой",
        "2Кривой",
        "Name",
        "Something",
        "Красный",
        "Тысяча",
        "Тыл",
        "Олово",
        "Оловянный",
        "Дом",
        "Домовенок"]

types =  ["Человек", 
        "Машина",
        "Знание",
        "Дерево",
        "Новое",
        "Что-то"]

for i in range(0, 100):
    name = names[random.randint(0, len(names) - 1)]
    x = round(random.uniform(-100.0, 100.0), 2)
    y = round(random.uniform(-100.0, 100.0), 2)
    tp = types[random.randint(0, len(types) - 1)]
    createTime = time.time() -random.uniform(-time.time(), time.time())
    line = f"{name} {x} {y} {tp} {createTime}\n"
    file.write(line)
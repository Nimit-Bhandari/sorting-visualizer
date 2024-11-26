import random
num_count = random.randint(20, 50)  
with open('randomscript.txt', 'w') as file:
    for _ in range(num_count):
        num_digits = random.randint(3, 5)
        random_number = random.randint(10**(num_digits-1), 10**num_digits - 1)
        file.write(f"{random_number}\n")

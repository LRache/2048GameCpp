f = open("./2024.2048game", "wb")
f.write(int.to_bytes(2024, 4, "little")) # score
f.write(int.to_bytes(2023, 4, "little")) # undo count
for i in [1, 3, 1, 2, 4, 5, 6, 7, 4, 8, 9, 10, 9, 11, 12, 13]:
    f.write(int.to_bytes(i, 4, "little"))
f.write(int.to_bytes(0, 1, "little"))
f.close()

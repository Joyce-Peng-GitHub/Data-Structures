def toBinStr(x: int) -> str:
    return "" if x == 0 else (toBinStr(x >> 1) + chr(48 ^ (x & 1)))


def toBaseUint64Max(x: int) -> list[int]:
    return [] if x == 0 else (toBaseUint64Max(x >> 64) + [x & ((1 << 64) - 1)])

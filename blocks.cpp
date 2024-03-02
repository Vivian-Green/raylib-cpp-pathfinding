#include "block.h"
#include "position.h"
#include <vector>

class LBlock : public Block {
public:
    LBlock() {
        id = 1;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {1, 0, 0}
            },
            {
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(0, 3);
    }
};

class JBlock : public Block {
public:
    JBlock() {
        id = 2;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {1, 1, 0}
            },
            {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(-1, 3);
    }
};

class IBlock : public Block {
public:
    IBlock() {
        id = 3;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(-1, 3);
    }
};

class OBlock : public Block {
public:
    OBlock() {
        id = 4;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            }
        };
        cells = BuildCells(blockTiles);
        Move(0, 4);
    }
};

class SBlock : public Block {
public:
    SBlock() {
        id = 5;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 0, 0},
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(0, 3);
    }
};

class TBlock : public Block {
public:
    TBlock() {
        id = 6;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(0, 3);
    }
};

class ZBlock : public Block {
public:
    ZBlock() {
        id = 7;

        std::vector<std::vector<std::vector<int>>> blockTiles = {
            {
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 0, 1},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {1, 0, 0}
            }
        };
        cells = BuildCells(blockTiles);
        Move(0, 3);
    }
};
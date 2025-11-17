

struct Vertex
{
    float x;
    float y;
};

struct PenStroke
{
    Vertex StartVertex;
    Vertex EndVertex;
    bool PenUpStatus;
};

struct CharData
{
    char characterEncoded;
    PenStrokes CharacterStrokes;
};

struct PenStrokes
{
    Vertex LocalOrigin;
    PenStroke* PenStrokes;
};
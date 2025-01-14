#ifndef CLASSES_H
#define CLASSES_H

// TODO: for the moment just integers
// TODO: Can alter have something like free all or similar, but would need tracker then
//


// not easy to make this generic
// can later become a union or something special where string is parsed
typedef struct {
    int shape0;
    int shape1;
    int shape2;
    int shape3;
} Shape;

typedef enum {
    INT,
    FLT,
    DBL
} Type;

typedef struct {
    int * array;
    int len; // TODO: should be shape or something
    int shape[5]; // experimental, but is this good?
} Array;

typedef struct {
    double * array;
    int len; // TODO: should be shape or something
    int shape[5]; // experimental, but is this good?
} ArrayD;

typedef struct {
    float * array;
    int len; // TODO: should be shape or something
    int shape[5]; // experimental, but is this good?
} ArrayF;

typedef struct {
    Array (*randint)(int len);
    ArrayF (*ranF)(int len);
    ArrayD (*ranD)(int len);
    int (*max)(Array array); // should take any array
    int (*min)(Array array);
    double (*scalar)(Array a1, Array a2);
    int ptr_counter;
    int len_ptr_list;
    int ** ptr_list;
} NumC;


NumC numcinit();
int clean_up(NumC * nc);
void add_ref(NumC * nc, Array * array);


#endif

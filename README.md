# Parallel Grade Averaging

Computes per-subject averages across a school by fanning the work out over a tree
of processes, one per level of the hierarchy, communicating through pipes and
named FIFOs.

## Requirements

A C++ compiler and `make`. POSIX processes and FIFOs, so Linux or macOS.

## Building

```bash
make
```

Produces four executables, one per level: `meanCalculator.out`, `class.out`,
`student.out` and `course.out`.

## Running

```bash
./meanCalculator.out school
```

`school/` holds one directory per class, each containing one CSV per student with
a `subject,score` line per subject. Subject processes print each average as it
arrives and keep listening, so stop the run with Ctrl-C when the output settles.

## The process tree

Each level forks the level below it and reads its results back through a pipe.

```
meanCalculator          walks the school, forks one child per class
  └── class             walks a class, forks one child per student
        └── student     reads one CSV, forks one child per subject
              └── course  receives scores on a named FIFO and averages them
```

Anonymous pipes carry results up the tree, from child back to the parent that
forked it. Named FIFOs carry them sideways: every student process writing a
Chemistry score writes to the same `Chemistry` FIFO, and one course process
reads them all. That is the part a plain pipe cannot do, since the writers are
not related to the reader by a fork.

The shape means each subject is averaged by exactly one process regardless of how
many students contribute to it, and no shared memory or locking is needed
anywhere.

## Project structure

```
meanCalculator.{h,cpp}  the root: walks the school, forks per class
class.{h,cpp}           forks per student in one class
student.{h,cpp}         reads one student's CSV, forks per subject
course.{h,cpp}          reads a subject FIFO and averages what arrives
libs.h                  shared includes
school/                 sample data, three classes of students
makefile                builds all four
```

## Data format

One CSV per student, named for the student, inside a directory named for the
class:

```
school/class3/ali.csv
    Physics,17.25
    English,14
    Math,17.5
```

import std.stdio;

void log(const(char)[] message)
{
    if (logging)
        fwritefln(logfile, message);
}

void foo(int i)
{
    log("Entering foo() with i set to " ~ toString(i));
}

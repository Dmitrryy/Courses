
class Integer2 {
    m_a : Integer;
    m_b : Integer;

    // method without return type
    method Print() {
        m_a.Print();
        m_b.Print();
    }

    // method with return type 'Integer2'
    method Set1(val : Integer) : Integer2 {
        m_a.Set(val);
        // If the return type is specified, then there must be 
        // a return at the end of the function.
        return this;
    }

    method Set2(val :Integer) : Integer2 {
        m_b.Set(val);
        return this;
    }
}

// Main class.
// Programm start with method Program.Program
class Program {
    m_int : Integer;
    m_int2 : Integer2;

    // It is not allowed to mix definitions of functions and fields of a class.
    //
    // Variables
    //=-----------------------
    // Methods

    method Print_all() {
        m_int.Print();
        m_int2.Print();
    }

    //=-----------------------
    // main
    method Program() {
        // print garbage. 'm_int' not initialized yet.
        m_int.Print();

        // We can build chain of expressions due to Set1 return object
        m_int2.Set1(99).Print();
        {
            loc : Integer;
            loc.Set(88);
            loc.Print();
        }
        //loc.Print(); // ERROR: loc undefined
    }
}

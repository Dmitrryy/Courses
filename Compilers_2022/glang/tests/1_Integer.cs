class Program {
    m_int : Integer;

    method arifmet() {
        a : Integer;
        b : Integer;
        c : Integer;
        a.Set(5);
        b.Set(7);

        // equal:
        // c.Set(a.Add(b));
        // c.Print();
        a.Add(b).Print();

        
    }

    method Get() : Integer {
        a : Integer;
        a.Set(10);
        return a;
    }

    method Program() {
        this.Get().Print();
        this.arifmet();
    }
}

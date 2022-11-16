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
        a.Sub(b).Print();
        a.Minus().Print();
        a.Mult(b).Print();
        a.Div(2).Print();
    }

    method cmptest(lhs : Integer, rhs : Integer) {
        lhs.Equal(rhs).Print();
        lhs.Equal(rhs.Add(1)).Print();
        lhs.NotEqual(rhs).Print();
    }

    method Get() : Integer {
        a : Integer;
        a.Set(10);
        return a;
    }

    method Program() {
        this.Get().Print();
        this.arifmet();
        this.cmptest(10, 10);
    }
}

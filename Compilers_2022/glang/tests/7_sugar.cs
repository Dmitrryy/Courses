class Program {
    m_a : Integer;
    
    method arifmet(a : Integer, b : Integer) {
        (a - b).Print();
        (a + b).Print();
        (a * b).Print();
        (b / a).Print();
        (-b).Print();
        (b - a * b).Print();
        (~b).Print();
        (a != b).Print();
    }


    method Program() {
        a : Integer(10);
        b : Integer(15);
        this.arifmet(a, b);
    }
}

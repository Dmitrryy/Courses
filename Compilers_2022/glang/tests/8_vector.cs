class Program {
    m_a : Integer;

    method fill(vec : Vector, n : Integer , val : Integer) {
        i : Integer(0);
        while (i < n) {
            vec.Set(i, val);
            i = i + 1;
        }
    }

    method print(vec : Vector, n : Integer) {
        i : Integer(0);
        while(i < n) {
            vec.Get(i).Print();
            i = i + 1;
        }
    }

    method Program() {
        // FIXME: vector don't have destructor =(
        vec : Vector(10);
        this.fill(vec, 6, 666);
        this.print(vec, 10);
    }
}

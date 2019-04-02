for (int i = 0; i < an_size; i++) { //theta(an)
    if (active_nodes[i].classif == -1) {
        classify(active_nodes[i], jobs);
    }
    if (min_classif > active_nodes[i].classif) {
        min_classif = active_nodes[i].classif;
        min_pos = i;
    }
}

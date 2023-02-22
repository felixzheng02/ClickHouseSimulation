Simulation::run() {
    if (arrival) {
        update queries' remaining sizes in processor by time-a;
        update time-c;
        generate a new query;
        n-jobs++;
        if there's space in processor {
            put the query in processor;
            n-jobs-proc++;
        } if not {
            put the query in queue;
        }
        update time;
        
        // no need to delete any queries from processor since time-a less than time-c;
        generate time-a;
    } if (completion) {
        update time-c;
        update time-a;
        update time;
        update queries' remaining sizes in processor by time-c;
        remove finished queries from processors;
        if there's queries waiting in queue {
            allocate queries;
        } if not {
            do nothing;
        }
    }
}

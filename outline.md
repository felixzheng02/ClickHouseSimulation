n-inelastic-proc, n-elastic-proc, (cores occupied by IP = n-inelastic-proc, other available cores = multiprogramming - n-inelastic-proc);

Simulation::run() {
    if (arrival) {
        update queries' remaining sizes in processor by time-a;
        update time-c;
        generate a new query;
        n-jobs++;

        if new query's first phase is elastic {
            if all cores are occupied by inelastic phases {
                put the query in queue;
            } if cores available (free or occupied by elastic phases {
                share cores;
            }
        } if inelastic {
            if there's one separate core available (not occupied by inelastic phases) {
                allocate;
            } if not {
            put the query in queue;
        }

        generate new time-a;
        update time;
        
    } if (completion) {
        update time-a;
        update queries' remaining sizes in processor by time-n;
        remove finished queries from processors or reallocate;
        update time-c;
        check space {
            if there's queries waiting in queue {
                allocate queries;
            } if not {
                do nothing;
            }
        }
        update time;
    }
}

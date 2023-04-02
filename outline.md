run() {
    time_next = min(time_arrival, time_completion)

    if (time_next == time_arrival) {

        update queries in processor, update time_completion

        generate a new query based on time_arrival

        allocate the query

        generate a new time_arrival

    } 
    if (time_next == time_completion) {

        update time_arrival

        update queries in processor {
            phase finishes
            reallocate cores
        }

        get queries from queue to allocate to processor

    }
}



SELECT people.name FROM people WHERE people.id IN(
    SELECT directors.person_id FROM directors WHERE directors.movie_id IN (
        SELECT ratings.movie_id FROM ratings WHERE ratings.rating >= "9.0"));

--SELECT count(people.name) from people WHERE people.id IN -- test
SELECT people.name from people WHERE people.id IN
    (SELECT stars.person_id FROM stars WHERE stars.movie_id IN
        (SELECT movies.id FROM movies WHERE movies.year = '2004'))
ORDER BY people.birth;

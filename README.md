# Datastore

We are going to provide you a well-formatted tsv file with some data, having the following columns: user_id, timestamp, event.

You have to make sure there is only 1 event per combination of `user_id, timestamp, event` in your system
(due to the nature of the pipeline producing events, you might receive the same event multiple times).
All indicated timestamps must be inclusive.

Your goal is to create this datastore which will be able to ingest the generated data and provide an API for doing aggregations.

- Create 3 endpoints:
  - `GET : /count?date_from=''&date_to=''&event=''&user_id=''`
  - `GET : /count_distinct_users?date_from=''&date_to=''&event=''`
  - `GET : /exists?&event=''&user_id=''`
- Possible aggregations:
  - `count` - simply count the number of events after filtering
    - Possible params:
      - `user_id` - optional
      - `date_from` - mandatory
      - `date_to` - mandatory
      - `event` - optional
  - `count_distinct_users` - get the number of unique user_ids recorded
    - Possible params:
      - `date_from` - mandatory
      - `date_to` - mandatory
      - `event` - optional
  - `exists` - whether a data point for this user_id and event_type exists
    - Possible params:
      - `user_id` - mandatory
      - `event` - mandatory
- Example dataset:
  ```
      | user_id | timestamp           | event     |
      |---------|---------------------|-----------|
      | 1       | 2021-01-01T00:00:00 | HOVER     |
      | 1       | 2021-01-01T00:01:00 | CLICK     |
      | 2       | 2021-01-01T00:03:00 | CLICK     |
      | 3       | 2021-01-01T00:06:00 | CLICK     |
      | 4       | 2021-01-01T00:09:00 | MOUSE_DOWN |
  ```
- The corresponding expected result, for some example queries:
    ```
        - **count**
            - GET  /count?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:10:00&&event=CLICK would return 3
            - GET  /count?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:10:00&&event=CLICK&user_id=1 would return 1
            - GET  /count?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:10:00 would return 5
        - **exists**:
            - GET  /exists?event=CLICK&user_id=1 would return true
            - GET  /exists?event=MOUSE_DOWN&user_id=1 would return false
            - GET  /exists?event=CLICK&user_id=4 would return false
        - **countDistinctUsers**:
            - GET  /count_distinct_users?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:10:00 would return 4
            - GET  /count_distinct_users?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:03:00&&event=MOUSE_DOWN would return 0
            - GET  /count_distinct_users?date_from=2021-01-01T00:00:00&date_to=2021-01-01T00:06:00&&event=CLICK would return 3
    ```
- Expected output: a JSON with the name of the aggregation and the integer value.
  - Ex: <br />
    &nbsp;&nbsp;&nbsp;&nbsp;`{"count": 100}`<br />
    &nbsp;&nbsp;&nbsp;&nbsp;`{"exists": false}`<br />
    &nbsp;&nbsp;&nbsp;&nbsp;`{"count_distinct_users": 30}`

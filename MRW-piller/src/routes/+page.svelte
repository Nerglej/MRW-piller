<script>
    import Entry from '$lib/utils/Entry.svelte';

    export let data
</script>

<h1>Data from people</h1>

<div class="list">
    {#each data.entries as entry}
        <Entry {entry}/>
    {/each}
</div>

<style>
    .list {
        display: flex;
        flex-wrap: wrap;
        flex-direction: row;
        gap: 1rem;
    }
</style>
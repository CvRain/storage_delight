import Client from "./client";
export default function Page() {
  return (
    <div className="flex w-full flex-col md:col-span-4">
      <h2 className='main'>
        TEst
        <Client a={12} />
      </h2>


      <hr />

      <p></p>
    </div>
  );
}